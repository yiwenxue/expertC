#include <stdio.h>
#include <string.h>

struct student 
{
       int id1;
       int id2;
       float percentage;
       char a;
       char b;
};

int main() 
{
    int i;
    struct student record1 = {1, 2, 90.5, 'A', 'B'};

    printf("size of structure in bytes : %d\n", 
                           sizeof(record1));

    printf("\nAddress of struct     = %u", &record1);
    printf("\nAddress of id1        = %u", &record1.id1 );
    printf("\nAddress of id2        = %u", &record1.id2 );
    printf("\nAddress of percentage = %u", &record1.percentage);
    printf("\nAddress of a          = %u", &record1.a );
    printf("\nAddress of b          = %u", &record1.b );

    return 0;
}
