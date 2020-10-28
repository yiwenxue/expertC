#ifndef __YIWEN_INTERPRETER__
#define __YIWEN_INTERPRETER__
#include "yiwen_fatal.h"

#define PROMPT_BUF 255 
#define CMD_BUF 255
static int inter_ifexit = 0;

static char cmd_line[CMD_BUF];
static char *cmd_token;
static char *cmd_args;

int inter_loop(int (*handle)());

char int_prompt[PROMPT_BUF] = ">";

int show_prompt()
{
    printf("%s", int_prompt);
    return 0;
}

int readlinen(char *str, int len)
{
    int i=0;
    char chr = 0;
    while (1) {
        if (i >= len){
            while ((fgetc(stdin)) != '\n');
            warning("Warning: Input line is larger than the buffer");
            break;
        }
        chr = fgetc(stdin);
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
    return i;
}

#endif
