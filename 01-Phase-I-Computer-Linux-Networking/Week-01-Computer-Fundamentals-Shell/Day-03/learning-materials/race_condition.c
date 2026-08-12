#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

volatile long long shared_counter = 0; // Shared counter variable

void* increment_loop(void* arg) {
    int thread_id = *((int*)arg);
    printf("Thread %d starting... %d\n", thread_id, *(int*)arg);
    for (int i = 0; i < 1000000; i++) {
        shared_counter++; // Increment the shared counter
    }
    printf("Thread %d finished.\n", thread_id);
    return NULL;
}

int main(){
    pthread_t thread1, thread2;
    int id1 = 1;
    int id2 = 2;

    printf("Main Process PID: %d\n", getpid());
    printf("Expected final counter value: 2000000\n");

    if(pthread_create(&thread1, NULL, increment_loop, &id1) != 0) {
        perror("Failed to create thread 1");
        return 1;
    }

    if(pthread_create(&thread2, NULL, increment_loop, &id2) != 0) {
        perror("Failed to create thread 2");
        return 1;
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Final counter value: %lld\n", shared_counter);

    return 0;
}
