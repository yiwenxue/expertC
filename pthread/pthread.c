#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

int i=0;

struct tdata{
    pthread_t id;
    void *data;
};

void *print(void *input){
    pthread_t id = ((struct tdata *)input)->id;
    printf("id: %ld joint.\n", id);
    while(1){
        if(i<1000)
            i++;
        else break;
        printf("id:%ld i: %d\n",id, i);
    }
    sleep(1);
    return NULL;
}

int main(int argc, char *argv[])
{
    int threadN = atoi(argv[1]);
    pthread_t *thread_id = (pthread_t*)malloc(sizeof(pthread_t) * threadN);

    for (int j = 0; j < threadN; ++j) {
        struct tdata data = {
            .id = j,
            .data = NULL
        };
        pthread_create(&thread_id[j], NULL, print, &data);
    }

    for (int j = 0; j < threadN; ++j) {
        pthread_join(thread_id[j], NULL);
    }
    
    return 0;
}
