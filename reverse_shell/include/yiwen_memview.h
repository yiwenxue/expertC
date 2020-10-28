#ifndef __YIWEN_MEMV__
#define __YIWEN_MEMV__

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
#endif
