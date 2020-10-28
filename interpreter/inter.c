#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "list.h"
#include "module.h"
#include "command.h"

#define BUFFER 255
#define ARGVBUFF 255

struct command_modules{
    char *cmd;
    char *des;
    int (*handle)(char *);
    LIST_ENTRY(command_modules) cmd_list;
};

struct runcmd_data_t{
    char *commands;
};

int exitf = 0;


int welcome(int type);
int show_prompt(char *prompt);
int readlinen(char *str, int len);
int command_phrease(char *cmd, char *arg);
int cmd_list_init();
int ghelp(char *arg);
int exit_intpr(char *arg){
    if (exitf == 0)
        exitf = 1;
    return 0;
}

int runcmd(char *arg){
    /* struct runcmd_data_t *uap */ 
    /*     = (struct runcmd_data_t*)arg; */
    char *uap = (char *)arg; 
#ifdef Debug
    printf("God say: %s\n",uap);
#endif 
    system(uap);
    return 0;
}


struct command_modules new[] = {
    {"load", "load a module", modload, {NULL, NULL}},
    {"modstat", "show mod info", modstat, {NULL, NULL}},
    {"unload", "unload a module", modunload, {NULL, NULL}},
    {"run", "run a bash command", runcmd, {NULL, NULL}},
    {"des", "show des of a mod ", NULL, {NULL, NULL}},
    {"help", "show this help", ghelp, {NULL, NULL}},
    {"exit","Exit the interpreter", exit_intpr, {NULL, NULL}},
    { NULL,                 NULL, NULL, {NULL, NULL}}
};

LIST_HEAD(mycmd, command_modules) cmdmod_head;

extern struct modlist m_list;

int main(int argc, char *argv[])
{
    char cmdline[BUFFER]; //= getchar();
    char prompt[BUFFER] = "> ";
    char *split;
    char *token;
    welcome(0);

    cmd_list_init();
    MODULE_LIST_INIT;
    if (argc == 2)
        modload(argv[1]);

    /**
     * Module init 
     * */

    while (!exitf) {
        /**
         * Show the prompt, which is configable.
         * To achieve this, just modify prompt.
         * */
        show_prompt(prompt);

        /**
         * Read a line of command into cmdline buffer
         * Note that the length is limited.
         * If taken EOF, exit.
         * */
        if (readlinen(cmdline, BUFFER) == -1)
            break;

        split = cmdline;
        token = strtok_r(split, " ,.", &split);

        command_phrease(token, split);

        /* if (cmd[0] == '\0'){ */
        /*     putchar('\n'); */
        /*     break; */
        /* } */
        /* system(cmd); */
    }
    struct module *var;
    LIST_FOREACH(var, m_listp, mod_list){
        var->moduledata->load(MOD_UNLOAD, 
                var->moduledata->extra);
        LIST_REMOVE(var, mod_list);
    }
    return 0;
}

int welcome(int type)
{
    printf("---------------\n");
    printf("Welcome h4ck3r!\n");
    printf("---------------\n");
    return 0;
}

int show_prompt(char *prompt)
{
    printf("%s", prompt);
    return 0;
}

int ghelp(char *arg){
    struct mycmd *cmdmod_headp = &cmdmod_head;
    struct command_modules *var = NULL;
    printf("Usage:\n");
    LIST_FOREACH_FROM(var, cmdmod_headp, cmd_list){
        printf("    %s", var->cmd);
        printf("    \t%s\n", var->des);
    }
    printf("\n");
    return 0;
}

int cmd_list_init(){

    struct mycmd *cmdmod_headp = &cmdmod_head;

    int index = 1;
    LIST_INIT(cmdmod_headp);
    LIST_INSERT_HEAD(cmdmod_headp, &new[0], cmd_list);

    while (new[index].cmd != NULL){
        LIST_INSERT_AFTER(&(new[index-1]), &(new[index]), cmd_list);
        index ++;
    }
    ghelp("nothing");

    return index;
}

int readlinen(char *str, int len)
{
    int i=0;
    char chr = 0;
    while (1) {
        if (i >= len){
            while ((fgetc(stdin)) != '\n');
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

int command_phrease(char *cmd, char *arg)
{
    if( cmd == NULL )
    {
        return 1;
    }
    struct mycmd *cmdmod_headp = &cmdmod_head;
    struct command_modules *var;

    LIST_FOREACH(var, cmdmod_headp, cmd_list){
        if (!strncmp(cmd, var->cmd, 255))
        {
            /* printf("%s - %s\n", cmd, args); */
            if (var->handle != NULL)
                var->handle((void *)arg);
            return 0;
        }
    }
    printf("command not found: %s\n try help\n", cmd);
    return 2;
}
