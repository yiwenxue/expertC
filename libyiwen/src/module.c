#include "../include/module.h"

/* The main module list head pointer used by the root program. */
struct mod_list_h *mod_list_main_hp;

int mprintf(const char * _fmt, ...){
    va_list ap;
    va_start(ap, _fmt);
    printf("       [module] ");
    int erro = vprintf(_fmt, ap);
    va_end(ap);
    return erro;
}

int modload(struct mod_list_h *mlist_hp,
        char *mod_name)
{
#if debug 
    printf("  [modload] Run the load routine.\n");
#endif 
    struct module *mod = modread(mod_name);
    if (mod == NULL){
        fprintf(stderr, "  [modload] On load module.\n");
        return -1;
    }
    /* printf("  [modload] module info:\n" */
    /*         "\t name: %s\n" */
    /*         "\t handle: %p\n" */
    /*         "\t extra: %p\n", mod->moduledata->name, mod->moduledata->load, mod->moduledata->extra); */

    int erro = modinit(mod);
    if (modappend(mlist_hp, mod) != 0){
        fprintf(stderr, "  [modload] On join module into a list.\n");
        return -1;
    }
    return 0;
}

int modunload(struct module *mod)
{
#if debug 
    printf("  [modunload] Run the unload routine.\n");
#endif 
    if (mod == NULL){
        fprintf(stderr, "  [modunload] On unload module.\n");
        return -1;
    }
    int erro = moduninit(mod);
    if (modpop(mod) != 0){
        fprintf(stderr, "  [modunload] On rm module from list.\n");
        return -1;
    }
    modfree(mod);
    return 0;
}

int modinit(struct module *mod){
#if debug 
    printf("  [modinit] Run the init routine.\n");
#endif 
    const struct module_data *mod_data = mod->moduledata;
    int erro = mod_data->load(MOD_LOAD, mod_data->extra);
    if (erro != 0){
        printf("  [modinit] %s module init faild.\n", mod->name);
    }
    return erro;
}

int moduninit(struct module *mod){
#if debug 
    printf("  [moduninit] Run the uninit routine.\n");
#endif 
    const struct module_data *mod_data = mod->moduledata;
    int erro = mod_data->load(MOD_UNLOAD, mod_data->extra);
    if (erro != 0){
        printf("  [moduninit] %s module uninit faild.\n", mod->name);
    }
    return erro;
}

/* Insert a module into a module list. */
int modappend(struct mod_list_h *mlist_hp,
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
        fprintf(stderr, "  [modread] Can not open dynamic module %s\n", modulename);
        if (dlerror() != NULL){
            fprintf(stderr, "  [modread] ");
            perror("");
            /* fprintf(stderr, "  [modread] Some errors appear on module %s\n", modulename); */
            /* dlclose(_handle); */
            return NULL;
        }
        return NULL;
    }

    struct module_data_wrap *wrap = 
        (struct module_data_wrap *)dlsym(_handle, "mod_data");
    const struct module_data *m_data = wrap->data; 

    /* If any abnormal behavior happend, close the handle, and unload the module. */
    if (dlerror() != NULL){
        fprintf(stderr, "  [modread] Some errors appear on module %s\n", modulename);
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
#if debug 
    printf("  [modfree] Run the modfree routine.\n");
#endif 
    int erro = dlclose(mod->modfile);
    if (erro != 0){
        fprintf(stderr, "  [modfree] mod %s dlfile cannot be closed .\n", mod->name);
        return erro;
    }
    free(mod);
    return 0;
}

void modshow(struct module *var){
    printf("------\n");
    printf(" name: %s\n", var->name);
    printf("modid: %d\n", var->modid);
    printf("\n");
}

int modctl_load(char *mod_name){
    return modload(mod_list_main_hp, mod_name);
}

int modctl_unload(char * mod_name){
    struct module *var, *tvar;
    int erro = 0;
    LIST_FOREACH_SAFE(var, mod_list_main_hp, m_list, tvar){
        if (strncmp(var->moduledata->name, mod_name, MOD_NAME_LEN) == 0){
            erro = modunload(var);
            return erro;
        }
    }
    return -1;
}
