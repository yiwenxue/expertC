/*-
 A tool used to show memory maps. It can also be used for
 network (TCP) dump.
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
