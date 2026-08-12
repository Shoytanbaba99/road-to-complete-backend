#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>

#define NUM_THREADS 4
#define BUFFER_SIZE 8192

unsigned long long total_newlines = 0;
pthread_mutex_t mutex;
typedef struct{
const char *filename;
off_t start_offset;
off_t end_offset;
unsigned long long local_count;
} ThreadArg;


void *count_chunk(void *arg){
    ThreadArg *targ= (ThreadArg *)arg;
    targ -> local_count = 0;

    int fd = open(targ-> filename, O_RDONLY);
    if(fd < 0){
        perror("open");
        exit(1);
    }
    off_t current_pos = targ->start_offset;
    off_t bytes_to_read = targ->end_offset - targ->start_offset;
    char buffer[BUFFER_SIZE];
    while(bytes_to_read > 0){
        size_t chunk_size = (bytes_to_read < BUFFER_SIZE) ? bytes_to_read : BUFFER_SIZE;
        if(bytes_to_read < 0){
            break;
        }
        ssize_t bytes_read = pread(fd, buffer, chunk_size, current_pos);
        if(bytes_read < 0){
            perror("read");
            exit(1);
        }
        for(ssize_t i = 0; i < bytes_read; i++){
            if(buffer[i] == '\n'){
                targ->local_count++;
            }
        }
        current_pos += bytes_read;
        bytes_to_read -= bytes_read;
    }
    close(fd);
    pthread_mutex_lock(&mutex);
    total_newlines += targ->local_count;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}


int main(int argc, char *argv[]){

    if(argc != 2){
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    const char *filename = argv[1];

    struct stat st;
    if(stat(filename, &st) != 0){
        perror("stat");
        exit(1);
    }
    off_t file_size = st.st_size;
    printf("Target File Size: %lld bytes (%.2f MB)\n", (long long)file_size, (double)file_size / (1024 * 1024));

    if(pthread_mutex_init(&mutex, NULL) != 0) {
        perror("pthread_mutex_init");
        exit(1);
    }

    pthread_t threads[NUM_THREADS];
    ThreadArg args[NUM_THREADS];
    off_t chunk_size = file_size / NUM_THREADS;
    for(int i = 0; i < NUM_THREADS; i++){
        args[i].filename = filename;
        args[i].start_offset = i * chunk_size;
        args[i].end_offset = (i == NUM_THREADS - 1) ? file_size : (i + 1) * chunk_size;
        args[i].local_count = 0;

        if(pthread_create(&threads[i], NULL, count_chunk, &args[i]) != 0){
            perror("pthread_create");
            exit(1);
        }
    }

    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);

    }
    pthread_mutex_destroy(&mutex);
    printf("Total Newlines: %llu\n", total_newlines);
}

