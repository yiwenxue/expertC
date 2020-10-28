#pragma once
#ifndef __YIWEN_MEMV__
#define __YIWEN_MEMV__
#include <stdio.h>
#include "yiwen_fatal.h"

void memv_byte(char *ptr, size_t size)
{
    if (ptr == NULL)
    {
        fatal("On print memory");
    }
    unsigned char * addr;
    printf("Content (char) in [%p, %p] byte-wise\n", ptr, ptr + size -1);
    for (size_t i=0; i<size; i++)
    {
        addr = ((unsigned char *)ptr + i);
        printf("%p\t0x%02x\n", addr, *addr);
    }
    putc('\n', stdout);
}

void memv_4byte(int *ptr, size_t size)
{
    if (ptr == NULL)
    {
        fatal("On print memory");
    }
    unsigned char * addr;
    printf("Content (int) in [%p, %p] byte-wise\n", ptr, ptr + size -1);
    for (size_t i=0; i<size; i++)
    {
        addr = ((unsigned char *)ptr + i * 4);
        printf("%p:\t0x%02x\t0x%02x\t0x%02x\t0x%02x\n", addr, *addr, *(addr +1), *(addr +2), *(addr +3));
    }
    putc('\n', stdout);
}

void memDump(char *desc, void *addr, int len) {
    int i;
    unsigned char buff[17];
    unsigned char *ptr = (unsigned char*)addr;

    if (desc != NULL)
        printf ("%s:\n", desc);
    for (i = 0; i < len; i++) {
        if ((i % 16) == 0) {
            if (i != 0)
                printf(" | %s\n", buff);
            printf("  %04x |", i);
        }
        printf(" %02x", ptr[i]);
        if ((ptr[i] < 0x20) || (ptr[i] > 0x7e)) {
            buff[i % 16] = '.';
        } else {
            buff[i % 16] = ptr[i];
        }
        buff[(i % 16) + 1] = '\0';
    }
    while ((i % 16) != 0) {
        printf("   ");
        i++;
    }
    printf(" | %s\n", buff);
}

#endif
