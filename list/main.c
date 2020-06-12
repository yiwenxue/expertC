#include <stdio.h>
#include <stdlib.h>
#include "mylist.h"

#define n_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define n_container_of(ptr, type, member)   \
    ((type *)((char *)(ptr) - offsetof(type, member)))

struct task{
    char *tit;
    int i;
    char *des;
    LIST_ENTRY(task) t_list;
};

#define MYSTR(str) #str

int main(int argc, char *argv[])
{
    if(argc == 1){
        printf("%s <num>\n",argv[0]);
        return 0;
    }
    printf("This is a list prototype for demo!\n");

    int num = atoi(argv[1]);

    /* struct task newtask; */

    /* newtask.tit = "Title"; */
    /* newtask.des = "Hey man, fuck you!"; */
    /* newtask.i = 1000; */


    /* int *myptr; */

    /* myptr = &newtask.i; */

    /* struct task *ptr; */
    /* ptr = container_of(myptr, struct task, i); */

    /* printf("addr[0] = %p\n", &newtask); */
    /* printf("addr[1] = %p\n", ptr); */

/* #if Debug */
    /* printf("------------\n"); */
    /* printf("offsetof 1: %lu\n", n_offsetof(struct task, des)); */
    /* printf("addr of tit: %p\n", &newtask.tit); */
    /* printf("addr of i: %p\n", &newtask.i); */
    /* printf("addr of des: %p\n", &newtask.des); */
    /* printf("------------\n"); */
    /* printf("offsetof 2: %lu\n", offset_of(struct task, des)); */
    /* printf("addr of tit: %p\n", &(ptr->tit)); */
    /* printf("addr of i: %p\n", &(ptr->i)); */
    /* printf("addr of des: %p\n", &(ptr->des)); */
    /* printf("------------\n"); */
/* #endif */
    /* printf("title: %s\n", ptr->tit); */
    /* printf("num: %d\n", ptr->i); */
    /* printf("des: %s\n", ptr->des); */

    /* char temp[255]; */

    LIST_HEAD(mytask, task) head;
    struct mytask *headp;
    struct task *var;
    headp = &head;

    var = (struct task*) malloc(sizeof(struct task));
    var->tit = (char *) malloc(sizeof(char) * 255);
    var->des = (char *) malloc(sizeof(char) * 255);
    sprintf(var->tit, "Fucking List");
    sprintf(var->des,  "The list to be fucked");
    /* var->t_list.le_next = NULL; */
    var->i = 0;
    /* var->t_list.le_prev = &head.lh_first; */
    /* LIST_FIRST(headp) = var; */

    LIST_INIT(headp);
    LIST_INSERT_HEAD(headp, var, t_list);

    for (int i = 1; i < num; i++) {
        var= (struct task *)malloc(sizeof(struct task));
        var->tit = (char *) malloc(sizeof(char) * 255);
        var->des = (char *) malloc(sizeof(char) * 255);
        sprintf(var->tit, "Fuck you (%d)",i);
        sprintf(var->des,  "Fuck you %d times",i);
        var->i = i;
        LIST_INSERT_HEAD(headp, var, t_list);
    }

    LIST_FOREACH(var, headp, t_list){
        if(var->i%2)
            printf("----------odd-----------\ntit: %s\ndes: %s\nnum: %d\n",var->tit, var->des, var->i);
        else
            printf("----------even----------\ntit: %s\ndes: %s\nnum: %d\n",var->tit, var->des, var->i);
    }

    printf("The deleted: [");
    LIST_FOREACH(var, headp, t_list){
        /* if(var->i%2 != 0) */
        /*     continue; */
            LIST_REMOVE(var, t_list);
            /* printf("********delete**********\ntit: %s\ndes: %s\nnum: %d\n",var->tit, var->des, var->i); */
            printf("%d ", var->i);
            if (var->i == 0)
                continue;
            free(var->des);
            free(var->tit);
            free(var);
    }
    printf("]\n");

    printf("The rest: [");
    LIST_FOREACH(var, headp, t_list){
        /* if(var->i%2) */
        /*     printf("----------odd-----------\ntit: %s\ndes: %s\nnum: %d\n",var->tit, var->des, var->i); */
        /* else */
        /*     printf("----------even----------\ntit: %s\ndes: %s\nnum: %d\n",var->tit, var->des, var->i); */
            /* printf("------------------------\ntit: %s\ndes: %s\nnum: %d\n",var->tit, var->des, var->i); */
            printf("%d ", var->i);
    }
    printf("]\n");


    return 0;
}
