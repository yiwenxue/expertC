#ifndef MY_MODULE
#define MY_MODULE

#include "list.h"

enum modact{
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
    struct moduledata_t *moduledata;
    LIST_ENTRY(module) mod_list;
};

LIST_HEAD(modlist, module) m_list;

struct modlist *m_listp = &m_list;

#define MODULE_METADATA(name, data) do{     \
    struct module *mymod = (struct module *)\
        malloc(sizeof(struct module));      \
    mymod->name = #name;                    \
    mymod->moduledata = data;               \
} while (0)

#define DECLARE_MODULE(name, data)  do{     \
    MODULE_METADATA(#name, &data);          \
} while (0)

#endif
