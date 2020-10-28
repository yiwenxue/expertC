#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include "include/list.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include "include/thread.h"
#include <signal.h>
#include "include/yiwen_fatal.h"
#include "include/yiwen_int.h"
#include "include/yiwen_memview.h"

#define READ_BUFFER 1024

typedef struct server_cb_t{
    int host_fd;
    struct sockaddr_in *host_addr;
} server_cb_t; 

thread_callback server_seeker;

typedef struct client_cb_t{
    int client_id;
    int client_fd;
    struct sockaddr_in *client_addr;
} client_cb_t ;

thread_callback client_cb;
thread_callback interpreter_cb;

struct server_cb_t *server_init(char *addr, int port);

void memDump(char *desc, void *addr, int len, char *prefix);
void exit_server();

int all_proc_lock = 0;
int client_proc_lock = 0;

LIST_HEAD(thread_pool, thread) mpool_head; // Create the main pool list;

int main(int argc, char *argv[])
{
    if (argc != 3 ){
        printf("Usage\n  <%s> <HOST> <PORT>\n", argv[0]);
        return -1;
    }
    int PORT = atoi(argv[2]);
    server_cb_t *host = server_init(argv[1], PORT);
    int host_fd = host->host_fd;
    struct sockaddr_in* host_addr = host->host_addr;

    struct thread_pool *mpool_hp = &mpool_head;
    LIST_INIT(mpool_hp);

    thread_t *thread_ptr[2];
    for (int i = 0; i < 2; ++i) {
        thread_ptr[i] = malloc(sizeof(thread_t));
    }
    THREAD_CREATE(thread_ptr[0], "interpreter", interpreter_cb, NULL);
    THREAD_CREATE(thread_ptr[1], "socket seeker", server_seeker, host);

    while (1){ 
        if (all_proc_lock == 0){
            all_proc_lock = 1;
            THREAD_JOIN(mpool_hp, thread_ptr[0]);
            THREAD_JOIN(mpool_hp, thread_ptr[1]);
            all_proc_lock = 0;
            break;
        }
        usleep(10);
    }

    thread_t *var;
    thread_t *var2;

    while(1) {
        if(mpool_hp->lh_first == NULL)
            break;
#if debug 
        printf("\nThreads: (lock=%d)\n", all_proc_lock);
        LIST_FOREACH(var, mpool_hp, t_list){
            printf("%10s\t|%2d\n", var->t_comm, var->t_state);
            if (var->t_children.lh_first != NULL){
                LIST_FOREACH(var2, &var->t_children, t_list){
                    client_cb_t *client_data = var2->extra;
                    printf("    |--%10s\tclient_fd: %d|%2d\n", var2->t_comm, client_data->client_fd, var2->t_state);
                }
            }
        }
#endif
        usleep(1e5);
    }
    return 0;
}

struct server_cb_t *server_init(char *addr, int port){
    int host_fd;
    struct sockaddr_in host_addr;
    host_addr.sin_addr.s_addr = inet_addr(addr);
    host_addr.sin_port = htons(port);
    host_addr.sin_family = PF_INET;
    memset(&host_addr.sin_zero, '\0', 8);

    host_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (host_fd == -1) {
        ERRL("%s\n","On creating socket");
        exit(-1);
    }
    int yes = 1;
    if (setsockopt(host_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
        ERRL("%s\n", "On setting socket parameters");
        exit (-1);
    }
    printf("[1] socket created!\n");

    if (bind(host_fd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1){
        ERRL("On binding socket to address: %s/%d\n", addr, port);
        exit (-1);
    }
    printf("[2] socket binded!\n");

    struct server_cb_t host={
        .host_fd = host_fd,
        .host_addr = &host_addr
    };

    if (listen(host_fd, 10) == -1){
        ERRL("%s", "On listening for incomming connections");
        exit (-1);
    }
    printf("[3] listen settled!\n");

    struct server_cb_t *host_r = malloc(sizeof(struct server_cb_t));
    memcpy(host_r, &host, sizeof(struct server_cb_t));
    return host_r;
}

void *client_cb(struct thread_data *data){
    thread_t *td = ((struct thread_data *)data)->td;
    struct client_cb_t *client_data = ((struct thread_data *)data)->data;
    assert(client_data == td->extra);

    td->extra = client_data;

    struct sockaddr_in *client_addr = client_data->client_addr;
    int client_id = client_data->client_id;
    int client_fd = client_data->client_fd;

    printf("  [Client] connected: %s/%d\n", inet_ntoa(client_addr->sin_addr), htons(client_addr->sin_port));
    printf("    Client ID: %5d\n", client_id);
    printf("    Thread ID: %5ld\n", pthread_self());
    printf("    File desc: %5d\n", client_fd);
    printf("    Data Addr: %p\n", client_data);

    // Listen to Client
    int recv_length = 0;
    char buffer[READ_BUFFER];

    send(client_fd, "Hello World!\n", 13, 0);
    recv_length = recv(client_fd, &buffer, READ_BUFFER, 0);
    while(recv_length > 0) {
        if (td->t_state == TRS_ZOMBIE)
            break;
        printf("\n    [Client %d] RECV: %d bytes\n", client_id, recv_length);
        memDump("buffer", buffer, recv_length, "    | ");
        printf("\n");
        recv_length = recv(client_fd, &buffer, 1024, 0);
    }
    close(client_fd);
    
    printf("  [Client] %d Will exit.\n", client_id);
    while (1){ 
        if (client_proc_lock == 0){
            client_proc_lock = 1;
            THREAD_EXIT(td);
            client_proc_lock = 0;
            break;
        }
        usleep(10);
    }
    printf("  [Client] %d Exit.\n", client_id);

    //cleaning
    if(td != NULL)
        free(td);
    if(client_addr != NULL)
        free(client_addr);
    if(client_data != NULL)
        free(client_data);
    return NULL;
}

void *server_seeker(struct thread_data *data){
    printf("[Server] launched!    Looking for clients.\n");
    thread_t *td = ((struct thread_data *)data)->td;
    struct server_cb_t *server_data= ((struct thread_data *)data)->data;

    int host_fd = server_data->host_fd;
    struct sockaddr_in *host_addr = server_data->host_addr;

    char client_name[255] = {0};

    fd_set client_set, read_fd;
    int fd_max = host_fd;
    int client_id = 0;

    while (1){
        if (td->t_state == TRS_ZOMBIE)
            break;
        FD_ZERO(&read_fd);
        FD_SET(host_fd, &read_fd);
        struct timeval timeout = {
            .tv_sec = 2,
            .tv_usec = 0
        };
        /* printf("host_fd: %5d|max_fd: %5d\n", host_fd, fd_max+1); */
        /* printf("waiting for new clients\n"); */
        int active = select(fd_max+1, &read_fd, NULL, NULL, &timeout);

        if (active == -1){
            perror("socket select");
            continue;
        }
        else if (active == 0){
            /* printf("time out!\n"); */
            continue;
        }

        socklen_t sin_size = sizeof(struct sockaddr_in);
        struct sockaddr_in *client_addr = malloc(sizeof(struct sockaddr_in));
        int client_fd = accept(host_fd, (struct sockaddr *)client_addr, &sin_size);
        if (client_fd == -1){
            ERRL("Cant accept connections%s", "");
        }
        thread_t *thread_ptr;
        thread_ptr = (thread_t *) malloc (sizeof(thread_t));

        client_cb_t client_data ={
            .client_id = client_id,
            .client_fd = client_fd,
            .client_addr = client_addr
        };

        client_cb_t *cbdata = malloc(sizeof(client_cb_t));
        memcpy(cbdata, &client_data, sizeof(client_cb_t));

        snprintf(client_name, 255, "client %d", client_id);
        THREAD_CREATE(thread_ptr, client_name, client_cb, cbdata);
        
        // lock
        while (1){
            if (client_proc_lock == 0){
                client_proc_lock = 1;
                THREAD_JOIN(&td->t_children, thread_ptr);
                client_proc_lock = 0;
                break;
            }
            usleep(10);
        }
        client_id ++;
    }

    /* printf("[Server] Seeker will exit.\n"); */
        // lock
    while (1){ 
        if (all_proc_lock == 0){
            all_proc_lock = 1;
            THREAD_EXIT(td);
            all_proc_lock = 0;
            break;
        }
        usleep(10);
    }
    printf("[Server] Seeker exit.\n");

    //cleaning
    if(td != NULL)
        free(td);
    if (server_data != NULL)
        free(server_data);

    return NULL;
}

void *interpreter_cb(struct thread_data *data){
    thread_t *td = ((struct thread_data *)data)->td;
    extern char int_prompt[];
    extern char *cmd_args;
    extern char *cmd_token;
    extern char cmd_line[];

    while (1) {
        show_prompt();
        if (readlinen(cmd_line, PROMPT_BUF) == -1){
            break;
        }
        strcat(cmd_line, "\n");

        if (strncmp(cmd_line, "exit", 4) == 0)
            break;
    }
    exit_server();
    while (1){ 
        if (client_proc_lock == 0){
            client_proc_lock = 1;
            THREAD_EXIT(td);
            client_proc_lock = 0;
            break;
        }
        usleep(10);
    }

    //cleaning
    if(td != NULL)
        free(td);
    return NULL;
}

void memDump(char *desc, void *addr, int len, char *prefix) {
    int i;
    unsigned char buff[17];
    unsigned char *ptr = (unsigned char*)addr;

    if (desc != NULL)
        printf ("%s%s:\n", prefix, desc);
    for (i = 0; i < len; i++) {
        if ((i % 16) == 0) {
            if (i != 0)
                printf(" | %s\n", buff);
            printf("%s %04x |", prefix, i);
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

void exit_server(){
    thread_t *var;
    thread_t *var2;
    LIST_FOREACH(var, &mpool_head, t_list){
        if (var->t_children.lh_first != NULL){
            LIST_FOREACH(var2, &var->t_children, t_list){
                var2->t_state = TRS_ZOMBIE;
                struct client_cb_t *client_datat = var2->extra;
                /* printf("[Client %s] Client id:%d\n", var2->t_comm, client_datat->client_id); */
                if (shutdown(client_datat->client_fd, SHUT_RDWR) != 0){
                    printf("[Client %d] client_fd: %d\n", client_datat->client_id, client_datat->client_fd);
                    perror("Closing client fd.");
                }
            }
        }
        var->t_state = TRS_ZOMBIE;
    }
}
