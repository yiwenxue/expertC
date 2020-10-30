/*-
 Dynamic loadable modular Framework.
 Copyright © 2020 yiwenxue
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

 THIS SOFTWARE IS PROVIDED BY yourname ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL yourname BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once
#ifndef MY_MODULE
#define MY_MODULE

#include "list.h"
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

/*-
 * Dynamic loadable module framework. Inspired by 
 * FreeBSD kernel module.
 *
 * This header provides a "protocal" to create dynamic 
 * loadable modules. A module list is needed to store
 * all information of a module. 
 *
 * A module shoule have a metadata struct that contains
 * 1. Name of this module; 
 * 2. The initializer function;
 * 3. A pointer to extra data for this module;
 * Note that: everything from the module file is read-
 * only for security;
 *
 * For any instance, an extra head is needed to put a
 * modular into module list;
 *
 * */

enum modstat{
    MOD_LOAD = 0,
    MOD_UNLOAD,
};

/* The DL module prototype. */
struct moduledata_t{
    const char              *name;          /* Name of the module. */
    int                     (*load)(int cmd ,void *);/* Initialize function of modules. */
    void                    *extra;         /* Extra data for modules. */
};

/* The modular list. created and maintained by main program.*/
struct module{
    const char              *name;          /* Name of the module.*/
    int                     modid;          /* Module id. */
    struct moduledata_t     *moduledata;    /* Module data, module information included. */
    LIST_ENTRY(module)      mod_list;       /* List of all modules. */
    void                    *modfile;       /* Pointer to module file */
};

int     modload(char *modulename);
int     modunload(char *modulename);
void    modshow(struct module *var);
int     modstat(char *arg);

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

/* Global variables */
LIST_HEAD(modlist, module)  m_list;         /* (m) header of module list. */
struct modlist              *m_listp = &m_list;/* (m) module list. */

static int                  modid = 0;      /* module counter, set modid for the next loaded module. */

/* def functions for module management. */
int modload(char *modulename){
    /* dl handler to load a module from file. 
     * memory leak will happen if not free with dlclose()
     * dlclose() should be invoked in modunload() function.*/
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

    /* It's not Ok to make the pointer confusing.*/
    struct module **newmod;
    newmod =(struct module **) dlsym(handle, "mymod");

#ifdef Debug
    printf("Module name: %s\n", (*newmod)->name);
#endif

    /* Initialization of a module: Invoking load() of the module. */
    (*newmod)->moduledata->load(MOD_LOAD,
            (*newmod)->moduledata->extra);
    /* Set modid for a module, which is an unique id. 
     * Id generator should be rewrited. */
    (*newmod)->modid = modid;
    (*newmod)->modfile = handle;

    /* If any abnormal behavior happend, close the handle, and unload the module. */
    if (dlerror() != NULL){
        fprintf(stderr, " MODULE: Can not close dynamic module %s\n", modulename);
        dlclose(handle);
        return 2;
    }

    LIST_INSERT_HEAD(m_listp, *newmod, mod_list);
    modid ++;

    return 0;
}

int modunload(char *modulename){
    struct module *var;
    LIST_FOREACH(var, m_listp, mod_list){
        if (strcmp(var->name, modulename) == 0){
            var->moduledata->load(MOD_UNLOAD,
                    var->moduledata->extra);
            LIST_REMOVE(var, mod_list);
            dlclose(var->modfile);
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
