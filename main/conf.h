#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef MYCONF
#define MYCONF

/*
 * confs: The data struct to store the profile
 * @name:   The name of targets
 * @path:   The pathe to the data file
 * @draw:   Draw a plot or not
 *
 * author: Yiwen Xue
 * date  : 2020-05-22
 * */
struct confs{
    char name[255];
    char path[255];
    int draw;
};

/*
 * setconf():  Function to set parameters of confs;
 * @myconf:      The struct to store the configuration;
 * @name :      The name to the parameter;
 * @value:      Set to this value;
 *
 * Return:  void
 *
 * author: Yiwen Xue
 * date  : 2020-05-22
 *
 * reviewer: none
 * date    ; none
 * */
void setconf(struct confs *myconf ,char *name, void *value)
{
    if(strcmp(name, "name") == 0){
        strncpy(myconf->name, (char*)value,255);
    }else if(strcmp(name, "path") == 0){
        strncpy(myconf->path, (char*)value,255);
    }else if(strcmp(name, "draw") == 0){
        myconf->draw = atoi((char*)value);
    }else{
        fprintf(stderr,"pattern not match!\n");
    }
}

void showconf(struct confs myconf){
    printf("myconf:\n{\n");
    printf(" name = %s,\n",myconf.name);
    printf(" path = %s,\n",myconf.path);
    printf(" draw = %d\n",myconf.draw);
    printf("}\n");
}

int readconf(struct confs *myconf, char *filename){

    char line[255];
    int linenum=0;
    char key[255], value[255];

    FILE *file = fopen(filename, "r");


    while(fgets(line, 256, file) != NULL)
    {
        linenum++;
        if(line[0] == '#') continue;

        if(sscanf(line, "%s %s", key, value) != 2)
        {
            continue;
        }

        setconf(myconf, key, value);
    }

    fclose(file);

    return 0;
}

#endif
