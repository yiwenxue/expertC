#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>


/* This is a structure that stores the options and the following */ 
/* array of strings are the description to these options */
/* {option-name,required_argument,args,short} */
static struct option long_options[]={
    {"file",    required_argument,  0,  'f'}, //iffile;
    {"filetype",required_argument,  0,  'T'}, //iffile;
    {"dfa",     required_argument,  0,  'd'}, //dfa
    {"mean",    no_argument,        0,  'm'}, //mean
    {"std",     no_argument,        0,  's'}, //std
    {"graph-type",required_argument,0,  't'}, //TYPE of diagram
    {"duration",required_argument,  0,  'D'}, //Time duration
    {"dlength", required_argument,  0,  'L'}, //single dfa length
    {"startp",  required_argument,  0,  'P'}, //start point
    {"graph-size",required_argument,0,  'S'}, //size of the graph
    {"output",  required_argument,  0,  'o'},
    {"help",    no_argument,        0,  'h'}, //helps
    {0,         0,                  0,  0  }
};
char *desc[]={
    "Name of the file to be analysised, followed by a file name",
    "The type of graph. [ MICE_DATA | MICE_DATA_38 | SINGLE_COLUM ]",
    "Exec dfa, followed by dfa order.",
    "Exec mean analysis",
    "Exec standard analysis",
    "The type of graph. [ GINDIVIDUAL | GAVERAGE | GDEVIATION | GMID | GMIN | GSYNC | GCCOR | GPOWERSPEC | GCHECK ]",
    "Set the duration of a single segment, 1 = 10sec (1080 = 3h)",
    "Set the whole length of individual dfa, 1 = 10sec",
    "Set the start point of individual dfa, 1 = 10sec",
    "Determint the dimensions of the graph",
    "Generate a pdf file for the graph, followed by a file name. If not seted, show a gnuplot window",
    "Show this help",
    ""
};

int print_usage(int argc, char **argv){
    printf("Usage: \n %s [OPTION]... [ARGS]... \n", argv[0]);
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
        print_usage(argc, argv);
        return -1;
    }

    int ifflag =0, iftypef =0, dfaf=0, meanf=0, stdf =0, gtypef =0,
        durationf =0, dlengthf =0, startpf =0, gsizef =0, outputf =0; /*flags hahah*/

    //default values
    int order = 1, width = 600, height = 450, duration = 1080, dlength = 1080, startp = 0;

    char ifname[255], outname[255], gtype[255], iftype[255], ifname2[255];

    /* Read parameters from command line; */
    int long_index = 0, opt = 0;
    while((opt = getopt_long(argc,argv,"f:T:d:mst:D:L:P:S:o:h",long_options,&long_index)) != -1){
        switch(opt){
            case 'f': ifflag = 1;
                      strcpy(ifname,optarg);
                      /* printf("ifflag = %d\tifname = %s\n",ifflag,ifname); */
                      break;
            case 'T': iftypef = 1;
                      strcpy(iftype,optarg);
                      /* printf("iftypef = %d\tiftype = %s\n",iftypef,iftype); */
                      break;
            case 'd': dfaf = 1;
                      meanf = 0;
                      stdf = 0;
                      order = atoi(optarg);
                      /* printf("meanf = %d\tstdf = %d\tcheckf = %d\tdfaf = %d\tdfa_order = %d\n",meanf,stdf,checkf,dfaf,order); */
                      break;
            case 'm': meanf = 1;
                      dfaf = 0;
                      stdf = 0;
                      /* printf("meanf = %d\tstdf = %d\tcheckf = %d\tdfaf = %d\tdfa_order = %d\n",meanf,stdf,checkf,dfaf,order); */
                      break;
            case 's': stdf = 1;
                      dfaf = 0;
                      meanf = 0;
                      /* printf("meanf = %d\tstdf = %d\tcheckf = %d\tdfaf = %d\tdfa_order = %d\n",meanf,stdf,checkf,dfaf,order); */
                      break;
            case 't': gtypef = 1;
                      strcpy(gtype,optarg);
                      /* printf("gtypef = %d\tgtype = %s\n",gtypef,gtype); */
                      break;
            case 'D': durationf = 1;
                      duration = atoi(optarg);
                      /* printf("durationf = %d\tduration = %d\n",durationf,duration); */
                      break;
            case 'L': dlengthf = 1;
                      dlength = atoi(optarg);
                      /* printf("dlengthf = %d\tdlength = %d\n",dlengthf,dlength); */
                      break;
            case 'P': startpf = 1;
                      startp = atoi(optarg);
                      /* printf("startpf = %d\tstartp = %d\n",startpf,startp); */
                      break;
            case 'S': gsizef = 1;
                      sscanf(optarg,"%dx%d",&width,&height);
                      /* printf("gsizef = %d\tsize = %dx%d\n",gsizef,width,height); */
                      break;
            case 'o': outputf =1;
                      strcpy(outname,optarg);
                      /* printf("outputf = %d\toutname = %s\n",outputf,outname); */
                      break;
            case 'h': print_usage(argc, argv);
                      break;
            default: print_usage(argc, argv);
                     return -1;
                     break;
        }
    }

    return 0;
}
