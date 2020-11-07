/*-
 A tool for string operations.
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

 THIS SOFTWARE IS PROVIDED BY yourname ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL yourname BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once
#include <string.h>
#include <ctype.h>
#include <stdio.h>

int unescape(char *input){
    const char esctable[256]= {
        ['a'] = '\a', ['b'] = '\b', ['f'] = '\f',
        ['n'] = '\n', ['r'] = '\r', ['t'] = '\t',
        ['v'] = '\v', ['\\'] = '\\', ['\''] = '\'',
        ['\"'] = '\"', ['?'] = '\?',
    };

    char *p = input, *q = input;
    while (*p) {
        int c = *(unsigned char *) p++;

        if (c == '\\') {
            c = *(unsigned char *)p ++;
            if (c == '\0') break;
            if (esctable[c] != 0)
                c = esctable[c];
        }
        *q++ = c;
    }

    *q = '\0';
    return q - input;
}

int unescapen(char *input, int length){
    const char esctable[256]= {
        ['a'] = '\a', ['b'] = '\b', ['f'] = '\f',
        ['n'] = '\n', ['r'] = '\r', ['t'] = '\t',
        ['v'] = '\v', ['\\'] = '\\', ['\''] = '\'',
        ['\"'] = '\"', ['?'] = '\?',
    };

    char *p = input, *q = input;
    int i=0;
    while (*p) {
        if (i++ > length)
            break;
        int c = *(unsigned char *) p++;

        if (c == '\\') {
            if (i++ > length)
                break;
            c = *(unsigned char *)p ++;
            if (c == '\0') break;
            if (esctable[c] != 0)
                c = esctable[c];
        }
        *q++ = c;
    }

    *q = '\0';
    return q - input;
}

void unescape_test(char *string){
    unescape(string);
    printf(" unescape: %s\n", string);
    return ;
}
void unescapen_test(char *string, int length){
    unescapen(string, length);
    printf("unescapen: %s\n", string);
    return ;
}

size_t split(char *buffer, char *argv[], size_t argv_size)
{
    char *p, *start_of_word;
    int c;
    enum states { DULL, IN_WORD, IN_STRING } state = DULL;
    size_t argc = 0;

    for (p = buffer; argc < argv_size && *p != '\0'; p++) {
        c = (unsigned char) *p;
        switch (state) {
            case DULL:
                if (isspace(c) || c < 32 || c > 127) continue;
                if (c == '"') {
                    state = IN_STRING;
                    start_of_word = p + 1; 
                    continue;
                }
                state = IN_WORD;
                start_of_word = p;
                continue;
            case IN_STRING:
                if (c == '"') {
                    *p = 0;
                    argv[argc++] = start_of_word;
                    state = DULL;
                } continue;
            case IN_WORD:
                if (isspace(c)) {
                    *p = 0;
                    argv[argc++] = start_of_word;
                    state = DULL;
                } continue;
        }
    }

    if (state != DULL && argc < argv_size)
        argv[argc++] = start_of_word;

    return argc;
}

int split_test(int argc, char **argv){
    if (argc < 2){
        printf("Usage:\n"
                "  <%s> <string|words>\n", argv[0]);
        return -1;
    }

    char *token[255] = {0};
    split(argv[1], token, 255);
    for (int i = 0; i < 255; ++i) {
        if ( token[i] != NULL )
            printf("    token[%d]: %s\n", i, token[i]);
    }

    return 0;
}
