#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../../include/yiwen_int.h"
#include "../../include/yiwen_string.h"
#include "../../include/command.h"
#include "../../include/module.h"
#include "../../include/log.h"
#include "../../include/yiwen_pcolor.h"

#define TBUFFER 1024
#define PATH_MAX 255

struct cmd_history {
    int id;
    char *commands;
    LIST_ENTRY(cmd_history);
};

void history_store(char *cmd, int len);
void history_store(char *cmd, int len){
    /* struct cmd_history * item = malloc(sizeof(struct cmd_history)); */
    /* item->commands = malloc(sizeof(char) * 1024); */
    /* timestamp_str(item->commands, 1024); */
    /* strncat(item->commands, cmd, len); */

    int fd = open("./.history", O_WRONLY|O_CREAT|O_APPEND, 0666);

    timestamp(fd);
    write(fd, cmd, len);
    write(fd, "\n", 1);

    close(fd);
}
void history_take(char *cmd, int len);
void history_list(){

}

int modctl(int argc, char **argv);
int inter_hist(int argc, char **argv);
int inter_hist(int argc, char **argv){
    return 0;
}

extern struct cmd_list_h *cmd_list_main_hp;
struct mod_list_h *mod_list_main_hp;

struct command_data builtin_cmd_data[] = {
    {"exit",        "exit the interpreter",                 inter_exit},
    {"help",        "show a brief introduce to commands",   inter_help},
    {"run",         "run a command through shell",          inter_run},
    {"history",     "show the commands you typed brfore",   inter_hist},
    {"modctl",      "module conctrl tool",                  modctl},
    {NULL, NULL, NULL}
};

struct command builtin_cmd[] = {
    {"exit",        "exit the interpreter",                 0, &builtin_cmd_data[0], {NULL, NULL}, NULL, NULL},
    {"help",        "show a brief introduce to commands",   0, &builtin_cmd_data[1], {NULL, NULL}, NULL, NULL},
    {"run",         "run a command through shell",          0, &builtin_cmd_data[2], {NULL, NULL}, NULL, NULL},
    {"hist",        "show the commands you typed brfore",   0, &builtin_cmd_data[3], {NULL, NULL}, NULL, NULL},
    {"modctl",      "module control tool",                  0, &builtin_cmd_data[4], {NULL,NULL}, NULL, NULL},
    {NULL,          NULL,                                   0, NULL, {NULL,NULL}, NULL, NULL}
};

extern int inter__state;
extern int inter_ifexit;

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


int modlist(struct mod_list_h *mod_list_hp){
    struct module *var;
    printf("List all module.\n");
    LIST_FOREACH(var, mod_list_hp, m_list){
        printf("%s at %p", var->name, var);
    }
    printf("\n");
    return 0;
}

