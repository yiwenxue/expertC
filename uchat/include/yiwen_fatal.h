#pragma once

#ifndef __YIWEN_FATAL__
#define __YIWEN_FATAL__

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "yiwen_pcolor.h"
#define FATAL_BUFFER 255

#define ERRL(_fmt, ...) fprintf(stderr, "[%s:%d]: " C_BOLDRED "error: " C_DEFAULT "" _fmt, __FILE__, __LINE__, __VA_ARGS__)

void fatal(char *message) {
    char error_msg[FATAL_BUFFER + 18];

    strcpy(error_msg, "[!!] Fatal Error: ");
    strncat(error_msg, message, FATAL_BUFFER);
    perror(error_msg);
    exit(-1);
}

void warning(char *message) {
    char error_msg[FATAL_BUFFER + 20];
    strcpy(error_msg, "[!!] Fatal Warning: ");
    strncat(error_msg, message, FATAL_BUFFER);
    perror(error_msg);
}

void *ec_malloc(unsigned int size) {
    void *ptr;

    ptr = malloc(size);
    if(ptr == NULL)
        fatal("in ec_malloc() on memory allocation");
    return ptr;
}
#endif
