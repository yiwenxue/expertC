#include "../../include/command.h"
#include "../../include/module.h"
#include "./screencap/data.h"
#include <stdio.h>
#include <string.h>

extern struct cmd_list_h *cmd_list_main_hp;
struct mod_list_h *mod_list_main_hp;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage:\n"
                "  <%s> <cmdmodule>\n", argv[0]);
        return -1;
    }
    printf("[main] launch\n");

    struct mod_list_h mod_list_main_h;
    mod_list_main_hp = &mod_list_main_h;
    LIST_INIT(mod_list_main_hp);

    struct cmd_list_h cmd_list_main_h;
    cmd_list_main_hp = &cmd_list_main_h;
    LIST_INIT(cmd_list_main_hp);

    printf("[main] header addr: %p\n", cmd_list_main_hp);
    modload(mod_list_main_hp, argv[1]);

    struct command *var;
    LIST_FOREACH(var, cmd_list_main_hp, cmd_list){
        if (strncmp(var->name, "screencap", 255) == 0){
            /* printf("    [cmd] screencap\n"); */
            char *arg[3] = {"screencap", ":1", "screen.png"};
            var->commanddata->evh(1, arg);
            /* if (var->handle == NULL || var->commanddata == NULL){ */
            /*     printf("    [cmd] data lost here!!\n"); */
            /* } */
        }
    }

    struct module *mvar, *tvar;
    LIST_FOREACH_SAFE(mvar, mod_list_main_hp, m_list, tvar){
        /* printf("[main] module info:\n" */
        /*         "\t name: %s\n" */
        /*         "\t handle: %p\n" */
        /*         "\t extra: %p\n", mvar->moduledata->name, mvar->moduledata->load, mvar->moduledata->extra); */
        modunload(mvar);
    }

    printf("[main] Exit!\n");
    
    return 0;
}
