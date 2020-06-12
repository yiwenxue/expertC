#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "module.h"
#include "list.h"

struct module *new;

int plist_len = 10;
struct moduledata_t *plist[10]; 

extern struct modlist *m_listp;

int loadmod(char *modulename){
    void *handle;

    int count = 0;
    if ((handle = dlopen(modulename, RTLD_LAZY)) == NULL){
        return 1;
    }

    new = dlsym(handle, "mymod");

    new->moduledata->load(MOD_LOAD,
            new->moduledata->extra);

    LIST_INSERT_HEAD(m_listp, new, mod_list);

    /* new->moduledata->load(); */

    if (dlerror() != NULL){
        dlclose(handle);
        return 2;
    }

    printf("Module name: %s\n", new->name);
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc != 2){
        printf("usae: %s <path to module>\n", argv[0]);
        return -1;
    }

    LIST_INIT(m_listp);

    /** plist init */
    for (int i = 0; i < plist_len; ++i) {
        plist[i] = NULL;
    }
    int load = 0;
    load = loadmod(argv[1]);
    if (load != 0){
        return load;
    }

    int j = 0;
    while(j < 10){
        printf("Main Loop\n");
        for (int i = 0; i < plist_len; ++i) {
            if (plist[i] != NULL){
                printf("ps: %s\n", plist[i]->name);
            }
        }
        j++;
    }

    struct module *var;
    LIST_FOREACH(var, m_listp, mod_list){
        var->moduledata->load(MOD_UNLOAD, 
                var->moduledata->extra);
        LIST_REMOVE(var, mod_list);
    }

    return 0;
}
