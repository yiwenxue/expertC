#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "conf.h"
#include "version.h"

static struct option long_options[]={
    {"conf",    required_argument,  0,  'c'},
    {"help",    no_argument,        0,  'h'},
    {"version", no_argument,        0,  'v'},
    {0,         0,                  0,  0  }
};

char *desc[] = {
    "Load config file",
    "Show this help msg",
    "Show the version of this program",
    ""
};


/* 
 * print_usage(): This function is used to generate a help menu.
 * @argc:   number of parameter main menu readin
 * @argv:   all the arguments readin at startup(to get the program's command.)
 *
 * Return:  0 the only return value, no use.
 *
 * author: Yiwen xue.
 * date  : 2020-05-22
 * reviewer: none
 * date    : none
 * */
int print_usage(int argc, char **argv){
    printf("Usage: \n %s <opt> <arg>\n", argv[0]);
    int i=0;
    while(1){
        if(long_options[i].name != NULL)
            printf("\t-%c, --%s\n\t    %s\n",long_options[i].val,long_options[i].name,desc[i]);
        else 
            break;
        i++;
    }
    return 0;
}



int main(int argc, char *argv[])
{
    if(argc == 1){
        fprintf(stderr, "no args received\n");
        print_usage(argc, argv);
        return -1;
    }

    int long_index = 0, opt = 0;
    struct confs thisconf;
    while((opt = getopt_long(argc,argv,
                    "c:hv",
                    long_options,&long_index)) != -1)
    {
        switch (opt) {
            case 'c': printf("Read in config: %s\n", optarg);
                      readconf(&thisconf, optarg);
                      showconf(thisconf);
                      break;
            case 'h': print_usage(argc, argv);
                      break;
            case 'v': printf("Version: %s\n",_version);
                      break;
            default : printf("Unexcepted input!\n");
                      print_usage(argc, argv);
                      return -1;
        }
    }

    return 0;
}
