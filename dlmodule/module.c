#include <stdio.h>
#include <string.h>
#include <module.h>

void print(void *args){
    char *uap = (char *)args;
    printf("mod: %s\n", uap);
}

struct module_data_t module_print = {"print", print};
