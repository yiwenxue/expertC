#include <stdio.h>
#include <stdlib.h>

struct mouse{
    char *name;
    int num;
};

void mouse_solve(void *args)
{
    struct mouse *uap;
    uap = (struct mouse *)args;

    printf("mouse name: %s\n",uap->name);
    printf("mouse   id: %d\n", uap->num);
}

int call_back( void (*func)(), void* input){
    (*func)(input);
    return 0;
}

int main(int argc, char *argv[])
{

    struct mouse newmouse = {"kaka", 10};

    call_back(mouse_solve, (void *)&newmouse);
    
    return 0;
}
