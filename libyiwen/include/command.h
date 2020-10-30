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

extern struct cmd_list_h *cmd_list_main_hp;

struct command_data_wrap {
    struct cmd_list_h   *cmd_list_hp;
    int                 (*handle)(int , void *);
    const struct command_data *cmd_data;
    void                *handle_args;
};

#define COMMAND_MODULE(name, data, evh, arg, list_hp)\
    const struct command_data_wrap  name##_cmd_data = {\
        .cmd_list_hp = list_hp,                   \
        .handle = evh,                              \
        .cmd_data = data,                           \
        .handle_args = arg};                        \
                                                    \
    static struct module_data name##_mod = {        \
        #name,                                      \
        command_env_handle,                         \
        (void *)&name##_cmd_data};                  \
                                                    \
    DECLARE_MODULE(name, name##_mod);


/* typedef int command_evh(int argc, char **argv); */

int command_env_handle(int cmd, void *extra);

int cmdload(struct cmd_list_h *, const struct command_data_wrap *wrap);
int cmdunload(struct command *cmd);

struct command *cmdfetch(const struct command_data_wrap *wrap);
int cmdfree(struct command *cmd);

int cmdinit(struct command *cmd);
int cmduninit(struct command *cmd);

int cmdappend(struct cmd_list_h *, struct command *cmd);
int cmdpop(struct command *cmd);
struct command *cmdseek(struct cmd_list_h *, const char *);
