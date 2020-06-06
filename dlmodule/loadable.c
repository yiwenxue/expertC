#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <module.h>

struct module_data_t *new;

int plist_len = 10;
struct module_data_t *plist[10]; 

int load_func(char *modulename){
    void *handle;

    int count = 0;
    if ((handle = dlopen(modulename, RTLD_LAZY)) == NULL){
        return 1;
    }

    new = dlsym(handle, "module_print");
    if (dlerror() != NULL){
        dlclose(handle);
        return 2;
    }

    plist[0] = new;   

    printf("Module name: %s\n", new->name);
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc != 2){
        printf("usae: %s <path to module>\n", argv[0]);
        return -1;
    }
    /** plist init */
    for (int i = 0; i < plist_len; ++i) {
        plist[i] = NULL;
    }
    int load = 0;
    load = load_func(argv[1]);
    if (load != 0){
        return load;
    }

    int j = 0;
    while(j < 10){
        printf("Main Loop\n");
        for (int i = 0; i < plist_len; ++i) {
            if (plist[i] != NULL){
                printf("ps: %s\n", plist[i]->name);
                plist[i]->handle((void *)"hello");
            }
        }
        j++;
    }
    return 0;
}
