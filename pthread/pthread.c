#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int i=0;

void *print(void *input){
    while(1){
        if(i<1000)
            i++;
        else break;
        printf("i: %d\n",i);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    int threadN = atoi(argv[1]);
    pthread_t *thread_id = (pthread_t*)malloc(sizeof(pthread_t) * threadN);

    for (int i = 0; i < threadN; ++i) {
        pthread_create(&thread_id[i], NULL, print, NULL);
    }

    for (int i = 0; i < threadN; ++i) {
        pthread_join(thread_id[i], NULL);
    }
    
    return 0;
}
