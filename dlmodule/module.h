#include <stdio.h>

struct module_data_t{
    char *name;
    void (*handle) (void *);
};
