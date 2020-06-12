#ifndef MY_MODULE
#define MY_MODULE

#include "list.h"
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

#define MODULE_LIST_INIT do{                \
    LIST_INIT(m_listp);                     \
} while (0)

#define MODULE_METADATA(name, data) do{     \
    struct module *var = (struct module *)  \
        malloc(sizeof(struct module));      \
    var->name = #name;                      \
    var->moduledata = data;                 \
} while (0)

#define DECLARE_MODULE(name, data)  do{     \
    MODULE_METADATA(#name, &data);          \
} while (0)

enum modstat{
    MOD_LOAD = 0,
    MOD_UNLOAD,
};

struct moduledata_t{
    const char *name;
    int (*load)(int cmd ,void *);
    void *extra;
};

struct module{
    const char *name;
    int modid;
    struct moduledata_t *moduledata;
    LIST_ENTRY(module) mod_list;
};

int modload(char *modulename);
int modunload(char *modulename);
void modshow(struct module *var);
int modstat(char *arg);

/* Global variables */
LIST_HEAD(modlist, module) m_list;
struct modlist *m_listp = &m_list;
int modid = 0;


/* def function */

int modload(char *modulename){
    void *handle;

    int count = 0;
    if ((handle = dlopen(modulename, RTLD_LAZY)) == NULL){
        fprintf(stderr, " MODULE: Can not open dynamic module %s\n", modulename);
        return 1;
    }
#ifdef Debug 
    else 
        printf("Module file: %s loaded successfully!\n", modulename);
#endif

    struct module **newmod;
    newmod =(struct module **) dlsym(handle, "mymod");

#ifdef Debug
    printf("Module name: %s\n", (*newmod)->name);
#endif
    (*newmod)->moduledata->load(MOD_LOAD,
            (*newmod)->moduledata->extra);
    (*newmod)->modid = modid;

    LIST_INSERT_HEAD(m_listp, *newmod, mod_list);
    modid ++;

    /* new->moduledata->load(); */

    if (dlerror() != NULL){
        fprintf(stderr, " MODULE: Can not close dynamic module %s\n", modulename);
        dlclose(handle);
        return 2;
    }

    return 0;
}

int modunload(char *modulename){
    struct module *var;
    LIST_FOREACH(var, m_listp, mod_list){
        if (strcmp(var->name, modulename) == 0){
            var->moduledata->load(MOD_UNLOAD,
                    var->moduledata->extra);
            LIST_REMOVE(var, mod_list);
            modid --;
            return 0;
        }
    }
    fprintf(stderr, "MODULE: mod %s not found!\n",modulename);
    return -1;
}

void modshow(struct module *var){
    printf("------\n");
    printf(" name: %s\n", var->name);
    printf("modid: %d\n", var->modid);
    printf("\n");
}

int modstat(char *arg){
    struct module *var;
    LIST_FOREACH(var, m_listp, mod_list){
        modshow(var);
    }
    return 0;
}

#endif
