#include <stdio.h>
#include "../../include/module.h"

static int 
load(int cmd, void *arg){
    int error = 0;

    switch (cmd){
        case MOD_LOAD:
            printf("Hello interpreter, I say loaded!\n");
            break;
        case MOD_UNLOAD:
            printf("Bye world, I say unloaded!\n");
            break;
        default:
            error = -1;
            break;
    }
    return error;
}

static const struct 
module_data isay_mod = {
    "isay",
    load,
    NULL
};

DECLARE_MODULE(isay, isay_mod);

