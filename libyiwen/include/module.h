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
#include <malloc.h>
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
struct module_data{
    const char              *name;          /* Name of the module. */
    int                     (*load)(int cmd ,void *);/* Initialize function of modules. */
    void                    *extra;         /* Extra data for modules. */
};
typedef struct module_data module_data_t;

struct module_data_wrap{
    const module_data_t *data;
};

/* The modular list. created and maintained by main program.*/
struct module{
    const char              *name;          /* Name of the module.*/
    unsigned int            modid;          /* Module id. */
    const struct module_data *moduledata;    /* Module data, module information included. */
    LIST_ENTRY(module)      m_list;       /* List of all modules. */
    void                    *modfile;       /* Pointer to module file */
};
typedef struct module module_t;

#define MODULE_METADATA(name, arg)         \
    struct module_data_wrap mod_data = {    \
        .data = arg};                  

#define DECLARE_MODULE(name, data)          \
    MODULE_METADATA(#name, &data);          

LIST_HEAD(m_list_h, module);             /* Create the list header prototype for modules. */

struct module *modread(char *);
int     modfree(struct module *mod);

int     modload(struct m_list_h *, char *);
int     modunload(struct module *);

void    modshow(struct module *);
int     modstat(char *arg);

int     modappend(struct m_list_h *, struct module *);
int     modpop(struct module *mod);

int     modinit(struct module *mod);
int     moduninit(struct module *mod);

int modload(struct m_list_h *mlist_hp,
        char *mod_name)
{
    struct module *mod = modread(mod_name);
    if (mod == NULL){
        fprintf(stderr, "On load module.\n");
        return -1;
    }
    int erro = modinit(mod);
    if (modappend(mlist_hp, mod) != 0){
        fprintf(stderr, "On join module into a list.\n");
        return -1;
    }
    return 0;
}

int modunload(struct module *mod)
{
    if (mod == NULL){
        fprintf(stderr, "On unload module.\n");
        return -1;
    }
    int erro = moduninit(mod);
    if (modpop(mod) != 0){
        fprintf(stderr, "On rm module from list.\n");
        return -1;
    }
    modfree(mod);
    return 0;
}

int modinit(struct module *mod){
    const struct module_data *mod_data = mod->moduledata;
    int erro = mod_data->load(MOD_LOAD, mod_data->extra);
    return erro;
}

int moduninit(struct module *mod){
    const struct module_data *mod_data = mod->moduledata;
    int erro = mod_data->load(MOD_UNLOAD, mod_data->extra);
    return erro;
}

/* Insert a module into a module list. */
int modappend(struct m_list_h *mlist_hp,
        struct module *mod)
{
    LIST_INSERT_HEAD(mlist_hp, mod, m_list);
    return 0;
}

int modpop(struct module *mod)
{
    LIST_REMOVE(mod, m_list);
    return 0;
}


/* def functions for module management. */
struct module* modread(char *modulename){
    /* dl handler to load a module from file. 
     * memory leak will happen if not free with dlclose()
     * dlclose() should be invoked in modunload() function.*/
    void *_handle;
    if ((_handle = dlopen(modulename, RTLD_LAZY)) == NULL){
        fprintf(stderr, " MODULE: Can not open dynamic module %s\n", modulename);
        return NULL;
    }
#ifdef Debug 
    else 
        printf("Module file: %s loaded successfully!\n", modulename);
#endif

    struct module_data_wrap *wrap = 
        (struct module_data_wrap *)dlsym(_handle, "mod_data");
    const struct module_data *m_data = wrap->data; 

#ifdef Debug
    printf("Module name: %s\n", m_data->name);
#endif

    /* If any abnormal behavior happend, close the handle, and unload the module. */
    if (dlerror() != NULL){
        fprintf(stderr, " MODULE: Can not close dynamic module %s\n", modulename);
        dlclose(_handle);
        return NULL;
    }

    struct module *__module = (struct module *) malloc (sizeof(struct module));
    __module->moduledata = m_data;
    __module->modfile = _handle;
    __module->name = m_data->name;

    return __module;
}

int modfree(struct module *mod){
    int erro = dlclose(mod->modfile);
    if (erro != 0){
        fprintf(stderr, "[Module] : mod %s dlfile cannot be closed .\n", mod->name);
        return erro;
    }
    free(mod);
    return 0;
}

/* int modunload(struct module *__module){ */
/*     int erro = dlclose(__module->modfile); */
/*     if (erro != 0){ */
/*         fprintf(stderr, "MODULE: mod %s dl file can not be freed!\n", __module->name); */
/*     } */
/*     return -1; */
/* } */

void modshow(struct module *var){
    printf("------\n");
    printf(" name: %s\n", var->name);
    printf("modid: %d\n", var->modid);
    printf("\n");
}

/* int modstat(char *arg){ */
/*     struct module *var; */
/*     LIST_FOREACH(var, m_listp, mod_list){ */
/*         modshow(var); */
/*     } */
/*     return 0; */
/* } */

#endif
