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
