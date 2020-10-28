#include "../include/yiwen_string.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 2){
        printf("Usage:\n"
                "  <%s> <string>\n", argv[0]);
        return -1;
    }
    pid_t pid = fork();
    if (pid == 0)
        unescape_test(argv[1]);
    else if (pid >0)
        unescapen_test(argv[1], strlen(argv[1]));
    else 
        printf("Fork failed!");
    return 0;
}
