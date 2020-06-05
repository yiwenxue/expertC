#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if(argc != 4){
        printf("Usae: \n"
                "\t%s <str1> <str2> <num>\n",argv[0]);
        return -1;
    }
    char *str1 = argv[1];
    char *str2 = argv[2];
    int num = atoi(argv[3]);
    int result = strncmp(str1, str2, num);

    printf("strncmp: %d\n",result);

    return 0;
}
