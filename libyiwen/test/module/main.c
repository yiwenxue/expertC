#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../../include/yiwen_int.h"
#include "../../include/yiwen_string.h"
#include "../../include/command.h"
#include "../../include/module.h"
#include "../../include/log.h"
#include "../../include/yiwen_pcolor.h"

#define TBUFFER 1024
#define PATH_MAX 255

void welcome();
int modctl(int argc, char **argv);

extern struct cmd_list_h *cmd_list_main_hp;
extern struct mod_list_h *mod_list_main_hp;

struct command_data builtin_cmd_data[] = {
    {"exit",        "exit the interpreter",                 inter_exit},
    {"help",        "show a brief introduce to commands",   inter_help},
    {"run",         "run a command through shell",          inter_run},
    {"modctl",      "module conctrl tool",                  modctl},
    {NULL, NULL, NULL}
};

struct command builtin_cmd[] = {
    {"exit",        "exit the interpreter",                 0, &builtin_cmd_data[0], {NULL, NULL}, NULL, NULL},
    {"help",        "show a brief introduce to commands",   0, &builtin_cmd_data[1], {NULL, NULL}, NULL, NULL},
    {"run",         "run a command through shell",          0, &builtin_cmd_data[2], {NULL, NULL}, NULL, NULL},
    {"modctl",      "module control tool",                  0, &builtin_cmd_data[3], {NULL,NULL}, NULL, NULL},
    {NULL,          NULL,                                   0, NULL, {NULL,NULL}, NULL, NULL}
};

extern int inter_state;
extern int inter_ifexit;

int main(int argc, char *argv[])
{
  if (argc < 2) {
    printf("Hello world\n");
  }
    /* if (argc < 2) { */
    /*     fprintf(stderr, "Usage:\n" */
    /*             "  <%s> <module>\n\n", argv[0]); */
    /* } */

#if  debug 
    printf("[main] Launch: Test routine for interpreter + module/command\n");
#endif 
    struct mod_list_h mod_list_main_h;
    mod_list_main_hp = &mod_list_main_h;
    LIST_INIT(mod_list_main_hp);

    struct cmd_list_h cmd_list_main_h;
    cmd_list_main_hp = &cmd_list_main_h;
    LIST_INIT(cmd_list_main_hp);

    if (argc >= 2)
    modload(mod_list_main_hp, argv[1]);

    for (int i = 0; 1; ++i) {
        if (builtin_cmd[i].name == NULL){
            break;
        }
        cmdappend(cmd_list_main_hp, &builtin_cmd[i]);
    }

    char cmdline[1024];
    welcome();

    while (!inter_ifexit) {
        show_prompt("[%t|%s] > ");

        if (readlinen(cmdline, TBUFFER) == -1){
            printf("\n");
            break;
        }
        parser(cmdline, TBUFFER);
    }
#if debug 
    printf("\n [inter] Done.\n");
#endif 
    struct module *mvar, *tvar;
    LIST_FOREACH_SAFE(mvar, mod_list_main_hp, m_list, tvar){
        /* printf("[main] module info:\n" */
        /*         "\t name: %s\n" */
        /*         "\t handle: %p\n" */
        /*         "\t extra: %p\n", mvar->moduledata->name, mvar->moduledata->load, mvar->moduledata->extra); */
        modunload(mvar);
    }
#if debug
    printf("[main] exit.\n");
#endif
    return 0;
}

void welcome(){
    char *asci = 
"                ```                     `.--.              \n"
"            -oyhhddddhs+-          ./shdddyyyyo-           \n"
"           --`    `:oddddh:      -hddy+:`      .-          \n"
"                     `+ddd:      `/:`                      \n"
"                       .hmdo                               \n"
"             `/oyyyso/. .mmm/         -+syyso/`            \n"
"          .+:hdmmmmmmdhsydmms       -ydmmmmmmmh:+.         \n"
"          .  -:://:-.    `ymh           `````    .         \n"
"                          /mm                              \n"
"                          ymm`                             \n"
"                        `ommm`                             \n"
"                       /mmmmm.                             \n"
"                  ..  -mmmmmms   ./+`   `-.                \n"
"          os+/os/:`    :++/hNNdhy--:`     ./ys+/yy         \n"
"          `y.`hmy`       .sNNNNNNm/        +Nm.`y.         \n"
"           .y``smdo///+ohmNNm/.yNNNmyo+++ohNh. s-          \n"
"            -s` -ymNNNmdddhs:---+yhdddmNNds:  o/           \n"
"             -y`  :yo/:::--.......------`    o/            \n"
"              .y.   :so/-`                  o/             \n"
"               `o:     .-:://+oo:-.       .o.              \n"
"                 -/         `dNy         -/                \n"
"                   -        -mNm.       ``                 \n"
"                            hNNNs                          \n"
"                            oNNN/                          \n"
"                            `mNm`                          \n"
"                             /N+                           \n"
"                              s`                           \n"
"                                                           \n";
    printf("%s%s", asci,
" --------------------------------------------------------- \n"
"|                      Welcome, "C_RED"h4ck3r"C_DEFAULT".                   |\n"
" --------------------------------------------------------- \n\n"
);
}

int modctl(int argc, char **argv){
    int erro = 0;
    if (argc < 2){
        printf("Usage:\n"
                "  <%s> <command> [option]\n"
                "     load   <path to module> load a module.\n"
                "     unload <module name>    unload a module.\n"
                "     status <module name>    show module status.\n"
                "     list                    list all modules.\n", argv[0]);
        return -1;
    }
    if (strncmp(argv[1], "load", 4) == 0){
        erro = modload(mod_list_main_hp, argv[2]);
    }
    return erro;
}



int modlist(struct mod_list_h * self){
  module_t *var;
  LIST_FOREACH(var, self, m_list){
	printf("modname: %s\n", var->name);
  }
  return 0;
}
