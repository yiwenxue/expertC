#include "../include/yiwen_int.h"

int inter_state = 0;
int inter_ifexit = 0;

int readlinen(char *str, int len)
{
    int i=0;
    char chr = 0;
    while (1) {
        if (i >= len){
            while ((fgetc(stdin)) != '\n');
            warning("Warning: Input line is larger than the buffer");
            break;
        }
        chr = fgetc(stdin);
        str[i] = chr;
        if (!(chr - '\n')){
            break;
        }
        if (chr == EOF){
            return -1;
        }
        i++;
    }
    str[i] = '\0';
    return i;
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
    if (inter_state == 0)
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

int inter_run(int argc, char **argv){
    if (argc < 2){
        printf("Usage:\n"
                "  <%s>    <bash command>\n", argv[0]);
        return -1;
    }
    char cmd[INT_CMD_BUFFER] = {0};
    for (int i = 1; i < argc; ++i) {
        strcat(cmd, argv[i]);
        strcat(cmd, " ");
    }

    inter_state = system(cmd);

    return 0;
}

int inter_help(int argc, char **argv){
    extern struct cmd_list_h *cmd_list_main_hp;
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

int inter_exit(int argc, char **argv){
    inter_ifexit = 1;
    return 0;
}

void parser(char *cmd, int len){
    if (strlen(cmd) == 0){
        inter_state = -1;
        goto parser_exit;
    }
    char cmdline[1024];
    memcpy(cmdline, cmd, len);

    unescapen(cmdline, INT_CMD_BUFFER);

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

    char fullcmd[1024] = {0};
    for (int i =0; i < argn; i++){
        strcat(fullcmd, args[i]);
        strcat(fullcmd, " ");
    }

    /* history_store(fullcmd, strnlen(fullcmd, 1024)); */

    if (argn != 0){
        struct command *var;
        LIST_FOREACH(var, cmd_list_main_hp, cmd_list){
            if (strncmp(var->name, args[0], 255) == 0){
                inter_state = var->commanddata->evh(argn, args);
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
