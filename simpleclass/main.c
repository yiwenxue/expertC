#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mouse.h>

int main(int argc, char *argv[])
{
    mouse *newmouse; 
    newmouse = mouse_create();
    strncpy(newmouse->name, "fangfang", MOUSE_BUF);
    strncpy(newmouse->path, "/usr/bin", MOUSE_BUF);
    newmouse->ifmut = 1;
    newmouse->batch = 5;

    printf("mouse name: %s\n", newmouse->name);
    printf("mouse path: %s\n", newmouse->path);
    printf("mouse ifmut: %d\n", newmouse->ifmut);
    printf("mouse batch: %d\n", newmouse->batch);

    mouse_free(newmouse);

    printf("freedom!\n");

    return 0;
}
