#include "../include/yiwen_int.h"
#include "../include/yiwen_string.h"
#include <stdio.h>
#include <unistd.h>

#define TBUFFER 1024
#define PATH_MAX 255

void show_pwd(){
   char cwd[PATH_MAX];
   if (getcwd(cwd, PATH_MAX) != NULL) {
       printf("%s", cwd);
   }
}

void show_prompt(char *pmpt){
    int len = strlen(pmpt);
    int i=0;
    while (i < len){
        if(pmpt[i++] == '%'){
            if (pmpt[i] == 'd'){
                show_pwd();
            }
            i++;
        } else {
            putc(pmpt[i-1], stdout);
        }
    }
}

void parser(char *cmd, int len){
    char cmdline[1024];
    memcpy(cmdline, cmd, len);

    unescapen(cmdline, TBUFFER);

    char *args[255];
    char argn;

    argn = split(cmdline, args, 255);
    for (int i=0; i<argn ; i++){
        printf("[%s] ", args[i]);
    }
    puts("");
}

int main(int argc, char *argv[])
{
    printf("Test routine for interpreter.\n");

    char cmdline[1024];

    while (1) {
        show_prompt(" [%d] > ");

        if (readlinen(cmdline, TBUFFER) == -1)
            break;


        parser(cmdline, TBUFFER);
    }


    printf("Done.\n");
    return 0;
}

