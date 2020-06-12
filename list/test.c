#include <stdio.h>

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) ((type *)((char *)(ptr) - offsetof(type, member)))


struct sample {
    int mem1;
    char mem2;
    int mem3;
};
int main(void)
{

struct sample sample1;

do {
printf("Address of Structure sample1 (Normal Method) = %p\n", &sample1);
printf("Address of Structure sample1 (container_of Method) = %p\n",
                        container_of(&sample1.mem3, struct sample, mem3));
}while(0);

return 0;
}
