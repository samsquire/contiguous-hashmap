#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_SIZE 1024

struct hashmap_key {
    char key[1024];
    int len;
};
struct hashmap_value {
    char value[1024];
};

struct hashmap {
    struct hashmap_key key[MAX_SIZE];       
    struct hashmap_value value[MAX_SIZE]; 
};
struct work_def {
    struct hashmap *hashmap;
    int running;
    int count;
};

unsigned long
hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

int set_hashmap(struct hashmap *hashmap, char key[], char value[]) {
    unsigned long hsh = hash(key) % MAX_SIZE;
    memcpy(&(*hashmap).key[hsh], key, MAX_SIZE); 
    memcpy(&(*hashmap).value[hsh], value, MAX_SIZE); 
}
struct hashmap_value get_hashmap(struct hashmap *hashmap, char key[]) {
    unsigned long hsh = hash(key) % MAX_SIZE;
    return (*hashmap).value[hsh];
}

void *clone_benchmark(void *args) {
    struct work_def *work = args;
    int current = 0;
    struct hashmap *hashmaps = calloc(MAX_SIZE, sizeof(struct hashmap));
    printf("Using %luGB for test.\n", MAX_SIZE * sizeof(struct hashmap) / 1024 / 1024 / 1024);
    while (work->running == 1) {
        memcpy(&hashmaps[current++], work->hashmap, sizeof(struct hashmap)); 
        current = current % MAX_SIZE; 
        work->count++;
    }
}

int main() {
    struct hashmap *hashmap = malloc(sizeof(struct hashmap));
    struct work_def *work = malloc(sizeof(struct work_def));
    work->running = 1;
    work->hashmap = hashmap;
    work->count = 0;
    set_hashmap(hashmap, "hello", "world");
    printf("Found item in hashmap: %s\n", get_hashmap(hashmap, "hello").value);
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, clone_benchmark, work);
    unsigned long hashmap_size = sizeof(struct hashmap);
    printf("Size of hashmap: %lu\n", hashmap_size);

    sleep(5);
    work->running = 0;

    printf("Benchmark run: %ldGB\n\n", (work->count * hashmap_size) / 1024 / 1024 / 1024);
    pthread_join(thread_id, NULL);

    sleep(600);
      
    return 0;
}


