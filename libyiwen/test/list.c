#include "../include/list.h"
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

struct noob{
    int id;
    char name[255];
    LIST_ENTRY(noob) n_list;
};

int main(int argc, char *argv[])
{
    LIST_HEAD(n_list_h, noob) npool_h;
    struct n_list_h *npool_hp = &npool_h;

    LIST_INIT(npool_hp);

    struct noob *var;

    for (int i = 0; i < atoi(argv[1]); ++i) {
        var = (struct noob *)malloc(sizeof(struct noob));
        var->id = i;
        LIST_INSERT_HEAD(npool_hp, var, n_list);
    }
    printf("Header first: %p\n", LIST_FIRST(npool_hp));
    LIST_FOREACH(var, npool_hp, n_list){
        printf("var id: %d\t", var->id);
        printf("self: %p\t", var);
        printf("var->prev: %p\tvar->next: %p\t", 
                var->n_list.le_prev,
                LIST_NEXT(var, n_list));
        printf("Head first: %p\n", &LIST_FIRST(npool_hp));
    }

    puts("");

    struct noob *tvar;
    LIST_FOREACH_SAFE(var, npool_hp, n_list, tvar){
        if (var->id != 2)
            continue;
        LIST_REMOVE(var, n_list);
        struct noob * avar;
        LIST_FOREACH(avar, npool_hp, n_list){
            printf("var id: %d\t", avar->id);
            printf("self: %p\t", avar);
            printf("var->prev: %p\tvar->next: %p\t", 
                    avar->n_list.le_prev,
                    LIST_NEXT(avar, n_list));
            printf("Head first: %p\n", &LIST_FIRST(npool_hp));
        }

        printf("var id: %d\n", var->id);
        free(var);
    }

    return 0;
}
