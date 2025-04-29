#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Initial settings
#define SIZE 5
int buffer[SIZE];
int in = 0, out = 0;
sem_t empty, full;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Producer function
void* producer(void* arg) {
    	while (1) {
		// Write a random number in buffer
        	int item = rand() % 100;

        	sem_wait(&empty);
        	pthread_mutex_lock(&mutex);
        	buffer[in] = item;
        	printf("Producer produced %d at buffer position %d\n", item, in);
        	in = (in + 1) % SIZE;
        	pthread_mutex_unlock(&mutex);
        	sem_post(&full);
        	
		sleep(1);
    	}
}

// Consumer function
void* consumer(void* arg) {
    	while (1) {
		// Get a number from buffer
        	sem_wait(&full);
        	pthread_mutex_lock(&mutex);
        	int item = buffer[out];
        	printf("Consumer consumed %d at buffer position %d\n", item, out);
        	out = (out + 1) % SIZE;
        	pthread_mutex_unlock(&mutex);
        	sem_post(&empty);

        	sleep(rand() % 5 + 1);
    }
}

int main() {
    	// Init
	srand(time(NULL));
    	sem_init(&empty, 0, SIZE);
    	sem_init(&full, 0, 0);

	// Create thread
    	pthread_t prod, cons;
    	pthread_create(&prod, NULL, producer, NULL);
    	pthread_create(&cons, NULL, consumer, NULL);
    	pthread_join(prod, NULL);
    	pthread_join(cons, NULL);

    	sem_destroy(&empty);
    	sem_destroy(&full);
    	pthread_mutex_destroy(&mutex);

    	return 0;
}
