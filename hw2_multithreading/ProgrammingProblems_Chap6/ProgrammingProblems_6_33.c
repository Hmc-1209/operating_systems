#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_RESOURCES 5
sem_t sem;

void* thread_func(void* arg) {
	int count = *(int*)arg;

	// Block the thread until all required resources available
	for (int i = 0; i < count; i++) {
		printf("Thread %ld waiting for resource %d...\n", pthread_self(), i + 1);
		sem_wait(&sem); // Get resource
		printf("Thread %ld acquired resource %d\n", pthread_self(), i + 1);
	}
	
	// Simulate the thread working here
	sleep(1);

	// Finished work, releasing resources
	for (int i = 0; i < count; i++) {
		sem_post(&sem);	// Release resource
		printf("Thread %ld released resource %d\n", pthread_self(), i + 1);
	}
	printf("Thread released %d resources. \n", count);
	return NULL;
}

int main() {
    pthread_t t1, t2;
    int r1 = 3, r2 = 4;

    // Init semaphore
    sem_init(&sem, 0, MAX_RESOURCES); 

    // Create two thread to simulate the resource snatch condition
    pthread_create(&t1, NULL, thread_func, &r1);
    pthread_create(&t2, NULL, thread_func, &r2);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    sem_destroy(&sem);
    return 0;
}



