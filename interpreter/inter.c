#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define BUFFER 255

int readline(char *str, int len)
{
    int i=0;
    char chr = 0;
    while (1) {
        if (i >= len){
            while ((getchar()) != '\n');
            break;
        }
        chr = getchar();
        str[i] = chr;
        if (!(chr - '\n')){
            break;
        }
        if (chr == EOF){
            return -1;
        }
        i++;
    }
    str[i] = '\0';
    return 0;
}

int main(int argc, char *argv[])
{
    char cmd[BUFFER]; //= getchar();
    while (1) {
        printf("# ");
        if (readline(cmd, BUFFER) == -1)
            break;
        /* if (cmd[0] == '\0'){ */
        /*     putchar('\n'); */
        /*     break; */
        /* } */
        printf("%s\n",cmd);
        system(cmd);
        putchar('\n');
    }
    return 0;
}
