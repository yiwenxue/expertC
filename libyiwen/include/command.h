/*- 
 A template for create command module.
 Copyright © 2020 yiwen xue
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 1. Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 3. All advertising materials mentioning features or use of this software
 must display the following acknowledgement:
 This product includes software developed by the organization.
 4. Neither the name of the organization nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY yiwen xue ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL yiwen xue BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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

int         command_env_handle(int cmd, void *extra);

int         cmdload(struct cmd_list_h *, const struct command_data_wrap *wrap);
int         cmdunload(struct command *cmd);

struct command *cmdfetch(const struct command_data_wrap *wrap);
int         cmdfree(struct command *cmd);

int         cmdinit(struct command *cmd);
int         cmduninit(struct command *cmd);

int         cmdappend(struct cmd_list_h *, struct command *cmd);
int         cmdpop(struct command *cmd);
struct command *cmdseek(struct cmd_list_h *, const char *);
