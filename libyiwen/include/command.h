#pragma once

#include "list.h"
#include "module.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct command_data {
    const char          *name;
    const char          *desc;
    int                 (*evh)(int , char **);
};

struct command {
    const char          *name;      /* The name of a command.*/
    const char          *desc;      /* A short description a command. */
    unsigned int        cmd_id;       /* The id of a command. */
    const struct command_data *commanddata; /* The data of the command, defined by the module. */
    LIST_ENTRY(command) cmd_list;
    int                 (*handle)(int , void*); /* Pointer to the init function of the module. */
    void                *handle_args; /* data for init handle. */
};
typedef struct command command_t;

LIST_HEAD(cmd_list_h, command);

struct command_data_wrap {
    char                *cmd_list_name;
    int                 (*handle)(int , void *);
    const struct command_data *cmd_data;
    void                *args;
};

#define COMMAND_MODULE(name, data, evh, arg, list_name)\
    const struct command_data_wrap  name##_cmd_data = {\
        .cmd_list_name = #list_name,                   \
        .handle = evh,                              \
        .cmd_data = data,                           \
        .args = arg};                               \
                                                    \
    static struct module_data name##_mod = {        \
        #name,                                      \
        command_handle,                             \
        (void*)&name##_cmd_data};                   \
                                                    \
    DECLARE_MODULE(name, name##_mod);


typedef int *command_evh(int argc, char **argv);

int command_handle(int cmd, void *extra);

int command_handle(int cmd, void *extra){
    int erro = 0;

    const struct command_data_wrap *wrap = (const struct command_data_wrap *)extra;
    /* struct cmd_list_h *cmd_list_hp = wrap->cmd_list_headp; */ 

    switch (cmd) {
        case MOD_LOAD:
            /* cmdload(); */
            break;
        case MOD_UNLOAD:
            break;
        default:
            erro = -1;
            break;
    }
    return erro;
}

int cmdload(struct cmd_list_h *, struct command_data_wrap *wrap);
int cmdunload(struct command *cmd);

struct command *cmdfetch(struct command_data_wrap *wrap);
int cmdfree(struct command *cmd);

int cmdinit(struct command *cmd);
int cmduninit(struct command *cmd);

int cmdappend(struct cmd_list_h *, struct command *cmd);
int cmdpop(struct command *cmd);

int cmdload(struct cmd_list_h *cmd_list_hp, struct command_data_wrap *wrap){
    struct command *cmd = cmdfetch(wrap);
    if (cmd == NULL){
        fprintf(stderr, "Fetch command failed!\n");
        return -1;
    }
    int erro = cmdinit(cmd);
    if (cmdappend(cmd_list_hp, cmd) != 0) {
        fprintf(stderr, "On join command into command list.\n");
        return -2;
    }
    return 0;
}

int cmdunload(struct command *cmd){
    if (cmd == NULL){
        fprintf(stderr, "On unload command: cmd points to NULL.\n");
        return -1;
    }
    int erro = cmduninit(cmd);
    if (cmdpop(cmd) != 0){
        fprintf(stderr, "On unload command: can't remove from list normally.\n");
        return -2;
    }
    return 0;
}

int cmdinit(struct command *cmd){
    int erro = cmd->handle(MOD_LOAD, cmd->handle_args);
    return erro;
}

int cmduninit(struct command *cmd){
    int erro = cmd->handle(MOD_UNLOAD, cmd->handle_args);
    return erro;
}

int cmdappend(struct cmd_list_h *cmd_list_hp, struct command *cmd){
    LIST_INSERT_HEAD(cmd_list_hp, cmd, cmd_list);
    return 0;
}

int cmdpop(struct command *cmd){
    LIST_REMOVE(cmd, cmd_list);
    return 0;
}

struct command *cmdfetch(struct command_data_wrap *wrap){
    const struct command_data *cmd_data = wrap->cmd_data;
    struct command * cmd = (command_t *) malloc(sizeof(command_t));
    cmd->commanddata = cmd_data;
    cmd->desc = cmd_data->desc;
    cmd->name = cmd_data->name;
    cmd->handle = wrap->handle;
    cmd->handle_args = wrap->args;

    return cmd;
}

int cmdfree(struct command *cmd){
    free(cmd);
    return 0;
}


