#ifndef MOUSE
#define MOUSE
#include <stdlib.h>

#define MOUSE_BUF 255

typedef struct {
    char *name;
    char *path;
    int batch;
    int ifmut;
}mouse;

mouse* mouse_create()
{
    mouse *newmouse;
    newmouse = (mouse *)malloc(sizeof(mouse));

    newmouse->name = (char *)malloc(sizeof(char) * MOUSE_BUF);
    newmouse->path = (char *)malloc(sizeof(char) * MOUSE_BUF);
    newmouse->batch = -1;
    newmouse->ifmut = -1;

    return newmouse;
}

int mouse_free(mouse* mdel)
{
    if (mdel == NULL)
        return -1;
    if (mdel->name != NULL)
        free(mdel->name);
    if (mdel->path != NULL)
        free(mdel->path);
    free(mdel);
    return 0;
}


#endif
