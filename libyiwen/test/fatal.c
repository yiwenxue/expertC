#include <stdio.h>
#include <stdlib.h>
#include "../include/yiwen_fatal.h"

void err_infunction(void){
    ERRL("%s\n", "Error in function\n");
}

int main(int argc, char *argv[])
{
    ERRL("%s\n", "Error appears");
    err_infunction();
    return 0;
}
