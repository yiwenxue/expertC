#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define Debug 1
int ifdebug = Debug;
#define _DEBUG if(ifdebug) 

/*
 * mice_confs: The data struct to store the profile
 * @name:   The name of mice
 * @path:   The pathe to the data file
 * @draw:   Draw a plot or not
 * */
struct mice_confs{
    char name[255];
    char path[255];
    int draw;
};

/*
 * mice_setconf():  Function to set parameters of mice_confs;
 * @mice_confs: The struct to store the configuration;
 * @name:       The name to the parameter;
 * @value:      Set to this value;
 *
 * Return:  void
 * */
void mice_setconf(struct mice_confs *myconf ,char *name, void *value)
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

void mice_showconf(struct mice_confs myconf){
    printf("myconf:\n{\n");
    printf(" name = %s,\n",myconf.name);
    printf(" path = %s,\n",myconf.path);
    printf(" draw = %d\n",myconf.draw);
    printf("}\n");
}

int mice_readconf(struct mice_confs *myconf, char *filename){

    char line[255];
    int linenum=0;
    char key[255], value[255];

    FILE *file = fopen(filename, "r");


    _DEBUG printf("## Read in conf file!\n");
    while(fgets(line, 256, file) != NULL)
    {
        linenum++;
        if(line[0] == '#') continue;

        if(sscanf(line, "%s %s", key, value) != 2)
        {
            _DEBUG printf("Syntax error, line %d\n", linenum);
            continue;
        }
        _DEBUG printf("Line %d:\t%s\t %s\n", linenum, key, value);

        mice_setconf(myconf, key, value);
    }
    _DEBUG printf("## Config reading in finished!\n\n");

    fclose(file);

    return 0;
}
