#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "include/yiwen_fatal.h"
#include "include/yiwen_memview.h"
#include "include/yiwen_int.h"

extern char int_prompt[];
extern char *cmd_args;
extern char *cmd_token;
extern char cmd_line[];

void memDump(char *desc, void *addr, int len) {
    int i;
    unsigned char buff[17];
    unsigned char *ptr = (unsigned char*)addr;

    if (desc != NULL)
        printf ("%s:\n", desc);
    for (i = 0; i < len; i++) {
        if ((i % 16) == 0) {
            if (i != 0)
                printf(" | %s\n", buff);
            printf(" %04x |", i);
        }
        printf(" %02x", ptr[i]);
        if ((ptr[i] < 0x20) || (ptr[i] > 0x7e)) {
            buff[i % 16] = '.';
        } else {
            buff[i % 16] = ptr[i];
        }
        buff[(i % 16) + 1] = '\0';
    }
    while ((i % 16) != 0) {
        printf("   ");
        i++;
    }
    printf(" | %s\n", buff);
}

int main(int argc, char **argv)
{
    if (argc != 3 ){
        printf("Usage\n  <%s> <HOST> <PORT>\n", argv[0]);
        return -1;
    }

    // Create listener
	int sockfd, new_sockfd;  // listen on sock_fd, new connection on new_fd
	struct sockaddr_in host_addr, client_addr;	// my address information
	socklen_t sin_size;
	int recv_length=1, yes=1;
	char buffer[1024];

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		fatal("in socket");
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		fatal("setting socket option SO_REUSEADDR");
	host_addr.sin_family = AF_INET;		 // host byte order
	host_addr.sin_addr.s_addr = inet_addr(argv[1]); // automatically fill with my IP
	host_addr.sin_port = htons(atoi(argv[2]));	 // short, network byte order
	memset(&(host_addr.sin_zero), '\0', 8); // zero the rest of the struct
	if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
		fatal("binding to socket");
    if (listen(sockfd, 5) == -1)
        fatal("listening on socket");
    sin_size = sizeof(struct sockaddr_in);
    new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
    if(new_sockfd == -1)
        fatal("accepting connection");
    printf("server: got connection from %s port %d\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    // Interpreter 
    sprintf(int_prompt, "%s > ", inet_ntoa(client_addr.sin_addr));
    int ifexit = 0;

    ERRL("Hey there!%s","\n");

    pid_t pid = fork();
    if(pid <0) {
        printf("Fork failed! Exit.\n");
        return -1;
    }else if (pid == 0){
        recv_length = recv(new_sockfd, &buffer, 1024, 0);
        while (recv_length > 0){
            printf("RECV: %d bytes\n", recv_length);
            memDump("buffer", buffer, recv_length);
            recv_length = recv(new_sockfd, &buffer, 1024, 0);
        }
        close (new_sockfd);
        printf("child exit\n");
    } else if (pid > 0){
        while (1) {
            show_prompt();
            if (readlinen(cmd_line, PROMPT_BUF) == -1){
                ifexit = 1;
                break;
            }

            strcat(cmd_line, "\n");

            send(new_sockfd, cmd_line, strlen(cmd_line), 0);
        }
        close(new_sockfd);
        printf("parent exit\n");
    }
    return 0;
}
