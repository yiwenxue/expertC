#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "include/yiwen_fatal.h"
#include "include/yiwen_memview.h"
#include "include/yiwen_int.h"

extern char int_prompt[];
extern char *cmd_args;
extern char *cmd_token;
extern char cmd_line[];

#define PIPE_W 1
#define PIPE_R 0

#define PIPE_BS 1024 

typedef struct {
    int pipe[2];
} tpipe_t;

int main(int argc, char **argv)
{
    sprintf(int_prompt, "> ");
    tpipe_t mp, tp;

    if (pipe(mp.pipe) == -1)
        fatal("On creating pipe.");
    if (pipe(tp.pipe) == -1)
        fatal("On creating pipe.");

    int m=0, n=0;
    char buffert[PROMPT_BUF];
    char bufferm[PROMPT_BUF];
    int ifexit = 0;

    ERRL("Hey there!%s","\n");

    pid_t pid = fork();
    if (pid == 0){
        /* close(mp.pipe[PIPE_R]); */
        close(tp.pipe[PIPE_W]);
        while (1){
            memset(buffert, 0, PROMPT_BUF);
            if (read (tp.pipe[PIPE_R], buffert, PROMPT_BUF)){
                if (strncmp(buffert, "child exit", 10) == 0){
                    break;
                }
                printf(" [Child %d] Receive: \"%s\"\n", n, buffert);
                cmd_args = buffert;
                cmd_token = strtok_r(cmd_args, " ,.", &cmd_args);

                usleep(5e5);

                write(mp.pipe[PIPE_W], "done", 4);
                printf(" [Child %d] Send: \"%s\"\n", n, "done");

                n++;
            }
        }
        close(tp.pipe[PIPE_R]);
        /* close(tp.pipe[PIPE_W]); */
        /* close(mp.pipe[PIPE_W]); */
    }
    if (pid > 0){
        close(tp.pipe[PIPE_R]);
        /* close(mp.pipe[PIPE_W]); */
        while (1) {
            show_prompt();
            if (readlinen(cmd_line, PROMPT_BUF) == -1){
                ifexit = 1;
                break;
            }

            printf("[Parent %d] Send: \"%s\"\n", m, cmd_line);
            if (strlen(cmd_line) == 0)
                continue;
            write(tp.pipe[PIPE_W], cmd_line, strlen(cmd_line));

            memset(bufferm, 0, PROMPT_BUF);
            read(mp.pipe[PIPE_R], bufferm, PROMPT_BUF);
            printf("[Parent %d] Receive: \"%s\"\n", m, bufferm);
            if (strncmp(bufferm, "done", 4) != 0)
                break;
            m++;
        }
        write(tp.pipe[PIPE_W], "child exit", 10);
        close(tp.pipe[PIPE_W]);
        /* close(tp.pipe[PIPE_R]); */
        /* close(mp.pipe[PIPE_R]); */
    }
    return 0;
}
