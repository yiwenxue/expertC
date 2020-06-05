#include <stdio.h>
#include <dlfcn.h>
#include <module.h>

struct module_data_t *new;

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

    printf("Module name: %s\n", new->name);
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc != 2){
        printf("usae: %s <path to module>\n", argv[0]);
        return -1;
    }
    int load = 0;
    load = load_func(argv[1]);
    if (load != 0){
        return load;
    }

    return 0;
}
