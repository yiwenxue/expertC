#include <stdio.h>
#include <module.h>
#include <list.h>

extern struct modlist * m_listp;

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

static struct 
moduledata_t isay_mod = {
    "isay",
    load,
    NULL
};

struct module *mymod  = &(struct module ){
    "isay",
    2,
    &isay_mod,
    {NULL, NULL}
};
