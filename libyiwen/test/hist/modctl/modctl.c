#include <stdio.h>
#include "../../../include/module.h"
#include "../../../include/command.h"

int plot(int argc, char **argv){
    if (argc < 2){
        printf("Usage:\n"
                "  <%s> <string>\n", "plot");
        return -1;
    }
    return 0;
}

const struct command_data plot_data = {
    .name = "plot",
    .desc = "fuck you",
    .evh = plot,
};

/* module_handle_t load; */
int 
load(int cmd, void *extra){
    int error = 0;

    switch (cmd){
        case MOD_LOAD:
            mprintf("Command Plot installed!\n");
            break;
        case MOD_UNLOAD:
            mprintf("Command Plot uninstalled!\n");
            break;
        default:
            error = -1;
            break;
    }
    return error;
}

/* struct */ 
/* module_data plot_mod = { */
/*     "plot", */
/*     load, */
/*     NULL */
/* }; */


COMMAND_MODULE(plot, &plot_data, load, NULL, NULL);
/* DECLARE_MODULE(plot, plot_mod); */
