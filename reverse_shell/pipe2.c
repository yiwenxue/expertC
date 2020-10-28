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

int main_loop(){

int main(int argc, char **argv)
{
    sprintf(int_prompt, "> ");
    int pipefd[2];

    if (pipe(pipefd) == -1)
        fatal("On creating pipe.");

    int m=0, n=0;
    char buffer[PROMPT_BUF];
    int ifexit = 0;

    pid_t pid = fork();
    if (pid == 0){
        while (1){
            close(pipefd[PIPE_W]);
            if (read (pipefd[PIPE_R], buffer, PROMPT_BUF)){
                printf("[Child %d] Receive command from pipe: \"%s\"\n", n, buffer);
                n++;
                close(pipefd[PIPE_R]);
            }
            printf("pid: %d\n", getpid());
            usleep(5e5);
            if (ifexit == 1){
                printf("Exiting!\n");
                break;
            }
        }
    }
    if (pid > 0){
        while (1) {
            printf("pid: %d\n", getpid());
            show_prompt();
            if (readlinen(cmd_line, PROMPT_BUF) == -1){
                ifexit = 1;
                break;
            }
            printf("[Parent %d] Send command to pipe: \"%s\"\n", m, cmd_line);
            close(pipefd[PIPE_R]);
            write(pipefd[PIPE_W], cmd_line, strlen(cmd_line));
            close(pipefd[PIPE_W]);
            cmd_args = cmd_line;
            cmd_token = strtok_r(cmd_args, " ,.", &cmd_args);
            /* printf("%s - %s\n", cmd_token, cmd_args); */
            m++;
        }
    }

    return 0;
}
