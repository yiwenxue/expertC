#include <stdio.h>
#include <module.h>
#include <list.h>

extern struct modlist * m_listp;

static int 
load(int cmd, void *arg){
    int error = 0;

    switch (cmd){
        case MOD_LOAD:
            printf("Hello interpreter, God say loaded!\n");
            break;
        case MOD_UNLOAD:
            printf("Bye world, God say unloaded!\n");
            break;
        default:
            error = -1;
            break;
    }
    return error;
}

static struct 
moduledata_t gsay_mod = {
    "gsay",
    load,
    NULL
};

struct module *mymod = & (struct module ){
    "gsay",
    1,
    &gsay_mod,
    {NULL, NULL}
};
