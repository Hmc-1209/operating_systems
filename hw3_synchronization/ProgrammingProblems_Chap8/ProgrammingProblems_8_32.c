#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_NORTHBOUND 5
#define NUM_SOUTHBOUND 5

// Initial settings
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
sem_t bridge;
int north_waiting = 0;
int south_waiting = 0;

// Northbound farmer func
void* northbound_farmer(void* arg) {
	pthread_mutex_lock(&lock);
	while (south_waiting > 0) {
		pthread_mutex_unlock(&lock);
		usleep(100000); // Sleep then check again
		pthread_mutex_lock(&lock);
	}
	north_waiting++;
	pthread_mutex_unlock(&lock);
	
	// Try to cross the bridge
	sem_wait(&bridge);
	printf("Northbound farmer crossing the bridge...\n");
	sleep(1);
	printf("Northbound farmer has crossed the bridge.\n");
	sem_post(&bridge);

	// Release
	pthread_mutex_lock(&lock);
	north_waiting--;
	pthread_mutex_unlock(&lock);
	return NULL;
}

// Southbound farmer func
void* southbound_farmer(void* arg) {
	pthread_mutex_lock(&lock);
    	while (north_waiting > 0) {
        	pthread_mutex_unlock(&lock);
  	      	usleep(100000); // Sleep then check again
        	pthread_mutex_lock(&lock);
    	}
    	south_waiting++;
   	 pthread_mutex_unlock(&lock);

	 // Try to cross the bridge
    	sem_wait(&bridge);
    	printf("Southbound farmer crossing the bridge...\n");
    	sleep(1);
    	printf("Southbound farmer has crossed the bridge.\n");
   	sem_post(&bridge);

	// Release
	pthread_mutex_lock(&lock);
    	south_waiting--;
    	pthread_mutex_unlock(&lock);
    	return NULL;
}

int main() {
 	srand(time(NULL));
    	pthread_t north[NUM_NORTHBOUND], south[NUM_SOUTHBOUND];

    	sem_init(&bridge, 0, 1);

    	// Create farmer threads
    	for (int i = 0; i < NUM_NORTHBOUND; i++) pthread_create(&north[i], NULL, northbound_farmer, NULL);
    	for (int i = 0; i < NUM_SOUTHBOUND; i++) pthread_create(&south[i], NULL, southbound_farmer, NULL);

    	// Join threads
    	for (int i = 0; i < NUM_NORTHBOUND; i++) pthread_join(north[i], NULL);
    	for (int i = 0; i < NUM_SOUTHBOUND; i++) pthread_join(south[i], NULL);

    	sem_destroy(&bridge);
    	pthread_mutex_destroy(&lock);
    	return 0;
}
