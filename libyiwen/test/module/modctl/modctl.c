#include "../../../include/command.h"
#include <stdio.h>

int modctl_modload(int argc, char **argv){
    if (argc < 2){
        printf("Usage:\n"
               "    modctl load [module file]\n");
        return -1;
    }

    return modctl_load(argv[1]);
}

int modctl_modunload(int argc, char **argv){
    if (argc < 2){
        printf("Usage:\n"
               "    modctl unload [module name]\n");
        return -1;
    }
    int erro = 0;
    erro = modctl_unload(argv[1]);
    if (erro == -1)
        mprintf("Module [%s] not found!\n", argv[1]);
    return erro;
}

int modctl(int argc, char **argv){
  if (argc < 2){
	printf("Usage:\n"
			"    <%s> <cmd> \n", argv[0]);
    return -1;
  }
  if (strncmp(argv[1], "load", 4) == 0) {
  	return modctl_modload(argc-1, argv+1);
  } else if (strncmp(argv[1], "unload", 6)){
  	return modctl_modunload(argc-1, argv+1);
  }
  return 0;
}

static int 
load(int cmd, void *extra){
  int erro =0;
  switch (cmd){
  case MOD_LOAD:
	mprintf("Modctl module installed\n");
	break;
  case MOD_UNLOAD:
	mprintf("Modctl module uninstalled\n");
	break;
  default:
	erro = -1;
	break;
  }
  return 0;
}

const struct command_data modctl_data = {
  .name = "modctl_new",
  .desc = "Modctl is cmd tool to load modules.",
  .evh = modctl
};

COMMAND_MODULE(modctl, &modctl_data, load, NULL, NULL);
