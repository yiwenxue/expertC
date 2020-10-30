#include "../include/command.h"

/* struct cmd_list_h cmd_list_main_h; */
struct cmd_list_h *cmd_list_main_hp;
/* = &cmd_list_main_h; */

int command_env_handle(int cmd, void *extra){
    int erro = 0;

    const struct command_data_wrap *wrap = (const struct command_data_wrap *)extra;
    struct cmd_list_h *cmd_list_hp = cmd_list_main_hp; 

    if (cmd_list_hp == NULL){
        erro = -1;
        return erro;
    }struct command *cmd_module;

    switch (cmd) {
        case MOD_LOAD:
            erro = cmdload(cmd_list_hp, wrap);
            /* mprintf("%s installed.\n", wrap->cmd_data->name); */
            break;
        case MOD_UNLOAD:
            cmd_module = cmdseek(cmd_list_hp, wrap->cmd_data->name);
            if (cmd_module == NULL || cmd_module->handle == NULL){
                printf("    [command_env_handle] %s module uninit -- handle func not found.\n", cmd_module->name);
                printf("      header addr: %p\n", cmd_list_hp);
                printf("      cmd module: %p\n"
                        "        name: %s\n"
                        "        handle: %p\n"
                        "        ", cmd_module, cmd_module->name, cmd_module->handle);
                return -1;
            }
            cmdunload(cmd_module);
            /* mprintf("%s uninstalled.\n", wrap->cmd_data->name); */
            break;
        default:
            erro = -1;
            break;
    }
    return erro;
}

struct command *cmdseek(struct cmd_list_h *cmd_list_hp, 
        const char *name)
{
    printf("    [cmdseek] Search the matched cmdmodule.\n");
    struct command *var;
    LIST_FOREACH(var, cmd_list_hp, cmd_list){
        if (strncmp(var->name, name, 255) == 0){
            if (var == NULL || var->handle == NULL){
                printf("    [cmdseek] %s module uninit -- data lost.\n", var->name);
                printf("      header addr: %p\n", cmd_list_hp);
                printf("      cmd module: %p\n"
                        "        name: %s\n"
                        "        handle: %p\n"
                        "        ", var, var->name, var->handle);
                return NULL;
            }
            return var;
        }
    }
    printf("    [cmdseek] the module %s not found!\n", name);

    return NULL;
}

int cmdload(struct cmd_list_h *cmd_list_hp, const struct command_data_wrap *wrap){
    printf("    [cmdload] Run the load routine.\n");
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
    printf("    [cmdunload] Run the unload routine.\n");
    if (cmd == NULL){
        fprintf(stderr, "On unload command: cmd points to NULL.\n");
        return -1;
    }
    if (cmd == NULL || cmd->handle == NULL){
        printf("    [cmdunload] %s module uninit -- data lost.\n", cmd->name);
        printf("      cmd module:\n"
               "        name: %s\n"
               "        handle: %p\n"
               "        ", cmd->name, cmd->handle);
        return -1;
    }
    int erro = cmduninit(cmd);
    if (cmdpop(cmd) != 0){
        fprintf(stderr, "On unload command: can't remove from list normally.\n");
        return -2;
    }
    cmdfree(cmd);
    return 0;
}

int cmdinit(struct command *cmd){
    printf("    [cmdinit] Run the init routine.\n");
    int erro = cmd->handle(MOD_LOAD, cmd->handle_args);
    if (erro != 0){
        printf("    [cmdinit] %s module init faild.\n", cmd->name);
    }
    return erro;
}

int cmduninit(struct command *cmd){
    printf("    [cmduninit] Run the uninit routine.\n");
    if (cmd == NULL || cmd->handle == NULL){
        printf("    [cmduninit] %s module uninit -- handle func not found.\n", cmd->name);
        printf("      cmd module:\n"
               "        name: %s\n"
               "        handle: %p\n"
               "        ", cmd->name, cmd->handle);
        return -1;
    }
    int erro = cmd->handle(MOD_UNLOAD, cmd->handle_args);
    if (erro != 0){
        printf("    [cmduninit] %s module uninit faild.\n", cmd->name);
    }
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

struct command *cmdfetch(const struct command_data_wrap *wrap){
    const struct command_data *cmd_data = wrap->cmd_data;
    struct command * cmd = (command_t *) malloc(sizeof(command_t));

    cmd->commanddata = cmd_data;
    cmd->desc = cmd_data->desc;
    cmd->name = cmd_data->name;
    cmd->handle = wrap->handle;
    cmd->handle_args = wrap->handle_args;

    return cmd;
}

int cmdfree(struct command *cmd){
    printf("    [cmdfree] Run the cmdfree routine.\n");
    free(cmd);
    return 0;
}
