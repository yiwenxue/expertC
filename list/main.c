#include <stdio.h>
#include <stdlib.h>
#include "mylist.h"

struct task{
    char *tit;
    int i;
    char *des;
    LIST_ENTRY(task) t_list;
};

#define MYSTR(str) #str

int main(int argc, char *argv[])
{
    printf("This is a list prototype for demo!\n");

    if(argc == 1){
        printf("%s <num>\n",argv[0]);
    }
    int num = atoi(argv[1]);

    char temp[255];

    LIST_HEAD(mytask, task) head;
    struct mytask *headp;
    struct task *var;
    headp = &head;

    var = (struct task*) malloc(sizeof(struct task));
    var->tit = (char *) malloc(sizeof(char) * 255);
    var->des = (char *) malloc(sizeof(char) * 255);
    sprintf(var->tit, "Fucking List");
    sprintf(var->des,  "The list to be fucked");
    var->t_list.le_next = NULL;
    var->i = 0;
    var->t_list.le_prev = var;

    LIST_FIRST(headp) = var;

    for (int i = num; i ; --i) {
        var= (struct task *)malloc(sizeof(struct task));
        var->tit = (char *) malloc(sizeof(char) * 255);
        var->des = (char *) malloc(sizeof(char) * 255);
        sprintf(var->tit, "Fuck you (%d)",i);
        sprintf(var->des,  "Fuck you %d times",i);
        var->i = i;
        LIST_INSERT_AFTER(head.lh_first, var, t_list);
    }

    LIST_FOREACH(var, headp, t_list){
        if(var->i%2)
            printf("----------odd-----------\ntit: %s\ndes: %s\nnum: %d\n",var->tit, var->des, var->i);
        else
            printf("----------even----------\ntit: %s\ndes: %s\nnum: %d\n",var->tit, var->des, var->i);
    }

    LIST_FOREACH(var, headp, t_list){
        /* if(var->i%2 == 0){ */
            LIST_REMOVE(var, t_list);
            printf("********delete**********\ntit: %s\ndes: %s\nnum: %d\n",var->tit, var->des, var->i);
            if (var->i == 0)
                continue;
            free(var->des);
            free(var->tit);
            free(var);
        /* } */
    }

    LIST_FOREACH(var, headp, t_list){
        /* if(var->i%2) */
        /*     printf("----------odd-----------\ntit: %s\ndes: %s\nnum: %d\n",var->tit, var->des, var->i); */
        /* else */
        /*     printf("----------even----------\ntit: %s\ndes: %s\nnum: %d\n",var->tit, var->des, var->i); */
            printf("------------------------\ntit: %s\ndes: %s\nnum: %d\n",var->tit, var->des, var->i);
    }


    return 0;
}
