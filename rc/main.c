#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "conf.h"

extern int ifdebug;


/**
 * main The entry point of this program
 * @argc:   how many input args
 * @argv:   array of string stores the args
 *
 * Returns: 0 if the program exit properly.
 *         -1 if error occurs.
 */
int main(int argc, char *argv[])
{
    if( argc != 2 ){
        printf("Usage: <%s> <file>\n", argv[0]);
        return -1;
    }

    ifdebug = 0;

    struct mice_confs myconf;

    mice_readconf(&myconf, argv[1]);
    
    mice_showconf(myconf);

    return 0;
}
