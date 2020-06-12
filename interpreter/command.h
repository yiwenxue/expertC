#ifndef MY_COMMANDS
#define MY_COMMANDS

#include "list.h"

struct command{
    char *cmd;
    char *des;
    int (*handle)(void *);
    LIST_ENTRY(command) cmd_list;
};

LIST_HEAD(cmdlist, command) c_list;

struct cmdlist *c_listp = &c_list;

#endif
