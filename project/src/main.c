#include <stdio.h>
#include <stdlib.h>
#include <fib.h>

int main()
{
    for (int i = 0; i < 10; ++i)
    {
        printf("fib(%d) = %d\n",i, fib(i));
    }
    return 0;
}
