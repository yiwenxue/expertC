#include "../../include/module.h"
#include <dlfcn.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
    if (argc < 2){
        printf("Usage:\n"
                "  <%s> <mod>\n", argv[0]);
        return -1;
    }
    void *_handle = dlopen(argv[1], RTLD_LAZY);
    if (_handle == NULL){
        perror(" ");
        printf("Can't open this dynamic lib.\n");
        return -1;
    }
    printf("Open this module successfully.\n");

    dlclose(_handle);
    return 0;
}
