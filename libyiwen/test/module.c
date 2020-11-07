#include "../include/module.h"
#include "../include/yiwen_memview.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
void modinfo(struct module *mod);

int main(int argc, char *argv[])
{
    struct mod_list_h mpool_h;
    struct mod_list_h *mpool_hp = &mpool_h;

    LIST_INIT(mpool_hp);

/*     assert(LIST_FIRST(mpool_hp) == NULL); */

    if (argc < 2){
        printf("Usage:\n"
                "  <%s> <modules>",
                argv[0]);
        return -1;
    }

    for (int i = 1; i < argc; ++i) {
        modload(mpool_hp, argv[i], NULL);
        /* struct module *mod = modload(argv[i]); */
        /* mod->modid = i; */
        /* mod->moduledata->load(MOD_LOAD, NULL); */
        /* modinfo(mod); */
        /* mod->moduledata->load(MOD_UNLOAD, NULL); */
        /* dlclose(mod->modfile); */
        /* free(mod); */
    }

    struct module *mod = NULL;
    struct module *tvar;
    LIST_FOREACH_SAFE(mod, mpool_hp, m_list, tvar){
        mod->modid = rand() %100;
        /* modinfo(mod); */
        modunload(mod, NULL);
    }

    return 0;
}

void modinfo(struct module *mod){
    printf("Mod info:\n");
    printf("name    :%16s\n", mod->name);
    printf("modid   :%16d\n", mod->modid);
    printf("moddata :%16p\n",  mod->moduledata);
    printf("    d_name  :%16s\n", mod->moduledata->name);
    printf("    d_hand  :%16p\n", mod->moduledata->load);
    printf("    d_extra :%16p\n", mod->moduledata->extra);
    printf("handle_p:%16p\n",  mod->modfile);
    memDump("module data", (void *)mod->moduledata, sizeof(struct module));
}
