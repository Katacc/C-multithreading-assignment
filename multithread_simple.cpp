#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define NUM_THREADS 5
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int *array;
    int start;
    int end;
} ThreadData;

void* fill_array(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    for (int i = data->start; i < data->end; i++) {
				pthread_mutex_lock(&mutex);
        data->array[i] = i;
				pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int size = 100000;
    int *taulukko = (int*)malloc(size * sizeof(int));
    if (taulukko == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Check if silent mode
    int silent_mode = 0;
    if (argc > 1 && strcmp(argv[1], "--silent") == 0) {
        silent_mode = 1;
    }

    // Set up threading
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    int segment_size = size / NUM_THREADS;

    // Create threads to fill segments of the array
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].array = taulukko;
        thread_data[i].start = i * segment_size;
        thread_data[i].end = (i == NUM_THREADS - 1) ? size : (i + 1) * segment_size; // Last thread covers the remainder
        pthread_create(&threads[i], NULL, fill_array, &thread_data[i]);
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print last 100 elements if not in silent mode
    if (!silent_mode) {
        for (int i = size - 100; i < size; i++) {
            printf("%d\n", taulukko[i]);
        }
    }

    free(taulukko);
    return 0;
}

