/*-
 A tool to print fatal informations 
 Copyright © 2020 yiwen xue
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 1. Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 3. All advertising materials mentioning features or use of this software
 must display the following acknowledgement:
 This product includes software developed by the organization.
 4. Neither the name of the organization nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY yiwen xue ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL yiwen xue BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#ifndef __YIWEN_FATAL__
#define __YIWEN_FATAL__

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
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
