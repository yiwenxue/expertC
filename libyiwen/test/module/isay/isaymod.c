#include <stdio.h>
#include "../../../include/module.h"
#include "../../../include/command.h"

void plot(){
    printf("I want you.\n");
}

int 
load(int cmd, void *extra){
    int error = 0;

    switch (cmd){
        case MOD_LOAD:
            mprintf("Hello interpreter, I say loaded!\n");
            break;
        case MOD_UNLOAD:
            mprintf("Bye world, I say unloaded!\n");
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
