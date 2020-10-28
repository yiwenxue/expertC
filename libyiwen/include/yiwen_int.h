#pragma once
#ifndef __YIWEN_INTERPRETER__
#define __YIWEN_INTERPRETER__
#include <stdio.h>
#include "yiwen_fatal.h"

#define BUFFER 255 
static int inter_ifexit = 0;

static char cmd_line[BUFFER];
static char cmd_token[BUFFER];
static char cmd_args[BUFFER];

int inter_loop(int (*handle)());

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
