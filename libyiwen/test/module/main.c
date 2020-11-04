#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../../include/yiwen_int.h"
#include "../../include/yiwen_string.h"
#include "../../include/command.h"
#include "../../include/module.h"
#include "../../include/log.h"
#include "../../include/yiwen_pcolor.h"

#define TBUFFER 1024
#define PATH_MAX 255

void show_pwd();
void show_time();
void show_prompt(char *pmpt);
void parser(char *cmd, int len);

int modctl(int argc, char **argv);
int inter_exit(int argc, char **argv);
int inter_help(int argc, char **argv);
int inter_run(int argc, char **argv);

extern struct cmd_list_h *cmd_list_main_hp;
struct mod_list_h *mod_list_main_hp;

struct command_data builtin_cmd_data[] = {
    {"exit",        "exit the interpreter",                 inter_exit},
    {"help",        "show a brief introduce to commands",   inter_help},
    {"run",         "run a command through shell",          inter_run},
    {"modctl",      "module conctrl tool",                  modctl},
    {NULL, NULL, NULL}
};

struct command builtin_cmd[] = {
    {"exit",        "exit the interpreter",                 0, &builtin_cmd_data[0], {NULL, NULL}, NULL, NULL},
    {"help",        "show a brief introduce to commands",   0, &builtin_cmd_data[1], {NULL, NULL}, NULL, NULL},
    {"run",         "run a command through shell",          0, &builtin_cmd_data[2], {NULL, NULL}, NULL, NULL},
    {"modctl",      "module control tool",                  0, &builtin_cmd_data[3], {NULL,NULL}, NULL, NULL},
    {NULL,          NULL,                                   0, NULL, {NULL,NULL}, NULL, NULL}
};

int shell_state = 0;
static int inter_ifexit = 0;

int main(int argc, char *argv[])
{
    /* if (argc < 2) { */
    /*     fprintf(stderr, "Usage:\n" */
    /*             "  <%s> <module>\n\n", argv[0]); */
    /* } */

#if  debug 
    printf("[main] Launch: Test routine for interpreter + module/command\n");
#endif 
    struct mod_list_h mod_list_main_h;
    mod_list_main_hp = &mod_list_main_h;
    LIST_INIT(mod_list_main_hp);

    struct cmd_list_h cmd_list_main_h;
    cmd_list_main_hp = &cmd_list_main_h;
    LIST_INIT(cmd_list_main_hp);

    if (argc >= 2)
    modload(mod_list_main_hp, argv[1]);

    for (int i = 0; 1; ++i) {
        if (builtin_cmd[i].name == NULL){
            break;
        }
        cmdappend(cmd_list_main_hp, &builtin_cmd[i]);
    }

    char cmdline[1024];

    while (!inter_ifexit) {
        show_prompt("[%t|%s] > ");

        if (readlinen(cmdline, TBUFFER) == -1)
            break;

        parser(cmdline, TBUFFER);
    }
#if debug 
    printf("\n [inter] Done.\n");
#endif 
    struct module *mvar, *tvar;
    LIST_FOREACH_SAFE(mvar, mod_list_main_hp, m_list, tvar){
        /* printf("[main] module info:\n" */
        /*         "\t name: %s\n" */
        /*         "\t handle: %p\n" */
        /*         "\t extra: %p\n", mvar->moduledata->name, mvar->moduledata->load, mvar->moduledata->extra); */
        modunload(mvar);
    }
#if debug
    printf("[main] exit.\n");
#endif
    return 0;
}

int modctl(int argc, char **argv){
    int erro = 0;
    if (argc < 2){
        printf("Usage:\n"
                "  <%s> <command> [option]\n"
                "     load   <path to module> load a module.\n"
                "     unload <module name>    unload a module.\n"
                "     status <module name>    show module status.\n"
                "     list                    list all modules.\n", argv[0]);
        return -1;
    }
    if (strncmp(argv[1], "load", 4) == 0){
        erro = modload(mod_list_main_hp, argv[2]);
    }

    return erro;
}

void show_pwd(){
   char cwd[PATH_MAX];
   if (getcwd(cwd, PATH_MAX) != NULL) {
       printf("%s", cwd);
   }
}

void show_time(){
    timestamp_std(stdout);
}

void show_shellstate(){
    if (shell_state == 0)
        printf(C_GREEN"Y"C_DEFAULT);
    else
        printf(C_RED"N"C_DEFAULT);
}

void show_prompt(char *pmpt){
    int len = strlen(pmpt);
    int i=0;
    while (i < len){
        if(pmpt[i++] == '%'){
            if (pmpt[i] == 'd'){
                show_pwd();
            } else if (pmpt[i] == 't'){
                show_time();
            } else if (pmpt[i] == 's'){
                show_shellstate();
            }
            i++;
        } else {
            putc(pmpt[i-1], stdout);
        }
    }
}

void parser(char *cmd, int len){
    if (strlen(cmd) == 0){
        shell_state = -1;
        goto parser_exit;
    }
    char cmdline[1024];
    memcpy(cmdline, cmd, len);

    unescapen(cmdline, TBUFFER);

    char *args[255] = {0};
    char argn;

    argn = split(cmdline, args, 255);
#if debug 
    printf("  [parser] %d ", argn);
    for (int i=0; i<argn ; i++){
        printf("<%d:%s> ", i, args[i]);
    }
    puts("");
#endif 
    if (argn != 0){
        struct command *var;
        LIST_FOREACH(var, cmd_list_main_hp, cmd_list){
            if (strncmp(var->name, args[0], 255) == 0){
                shell_state = var->commanddata->evh(argn, args);
                /* if (var->handle == NULL || var->commanddata == NULL){ */
                /*     printf("    [cmd] data lost here!!\n"); */
                /* } */
                goto parser_exit;
            }
        }
    }

/* #if debug */ 
    printf("  [parser] Command not found: %s\n", cmdline);
/* #endif */
parser_exit:
    return ;
}

int modlist(struct mod_list_h *mod_list_hp){
    struct module *var;
    printf("List all module.\n");
    LIST_FOREACH(var, mod_list_hp, m_list){
        printf("%s at %p", var->name, var);
    }
    printf("\n");
    return 0;
}

int inter_exit(int argc, char **argv){
    inter_ifexit = 1;
    return 0;
}

int inter_help(int argc, char **argv){
    if (argc == 1){
        printf("Usage:\n");
        struct command *var;
        LIST_FOREACH(var, cmd_list_main_hp, cmd_list){
            printf("    %-10s   %s\n", var->name, var->desc);
        }
    } else if (argc == 2){
        if (strncmp(argv[1], "help", 4) == 0 || strncmp(argv[1], "-h", 2) == 0){
            printf("Usage:\n"
                    "   <%s>    command      show the help information of a specific command.\n"
                    "             -h | help    show this help page.\n",
                    argv[0]);
        } else {
            struct command *var;
            LIST_FOREACH(var, cmd_list_main_hp, cmd_list){
                if (strncmp(var->name, argv[1], strlen(argv[1])) == 0)
                    var->commanddata->evh(1, argv+1);
            }
        }
    }
    return 0;
}

#include <sys/wait.h>

int inter_run(int argc, char **argv){
    if (argc < 2){
        printf("Usage:\n"
                "  <%s>    <bash command>\n", argv[0]);
        return -1;
    }
    char cmd[TBUFFER] = {0};
    for (int i = 1; i < argc; ++i) {
        strcat(cmd, argv[i]);
        strcat(cmd, " ");
    }

    shell_state = system(cmd);

    /* pid_t parent = getpid(); */
    /* pid_t pid = fork(); */

    /* if (pid == -1){ */
    /*     printf("  [run] Failed to run command.\n"); */
    /* } */ 
    /* else if (pid > 0){ */
    /*     int status; */
    /*     waitpid(pid, &status, 0); */
    /* } */
    /* else { */
    /*     execve("/bin/sh", NULL, NULL); */
    /* } */
    return 0;
}
