#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int *fib_seq;
int current = 0;
int length;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Function for child process
void* fib_func(void* arg) {
	// Set up the first two elements in fib seqence
	fib_seq[0] = 0;
	if (length > 1) {
		fib_seq[1] = 1;
		pthread_mutex_lock(&lock);
		current = 1;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&lock);
	}

	for (int i = 2; i < length; i++) {
		fib_seq[i] = fib_seq[i - 1] + fib_seq[i - 2]; // Claculate fib
		pthread_mutex_lock(&lock);
		current = i;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&lock);
	}

	return NULL;
}

// Main process function
int main(int argc, char* argv[]) {
	// Check input args
	if (argc != 2) {
		printf("Usage: %s <length_of_fibonacci_sequence>\n", argv[0]);
		return 1;
	}

	// Check input length
	length = atoi(argv[1]);
	if (length <= 0) {
		printf("Please enter a positive integer.\n");
		return 1;
	}

	// Allocate memory space
	fib_seq = malloc(length * sizeof(int));
	if (fib_seq == NULL) {
		perror("Memory allocation failed!\n");
		return 1;
	}

	// Create child process
	pthread_t tid;
	pthread_create(&tid, NULL, fib_func, NULL);

	// Wait until correct starting number being set
	pthread_mutex_lock(&lock);
	while (current < 1) {
		pthread_cond_wait(&cond, &lock);
	}
	pthread_mutex_unlock(&lock);

	// Print out the results
	printf("Fibonacci sequence: ");
	if (length >= 1) printf("%d ", fib_seq[0]);
	if (length >= 2) printf("%d ", fib_seq[1]);
	for (int i = 2; i < length; i++) {
		// Wait for fib to be calculated
		pthread_mutex_lock(&lock);
		while (current < i) {
			pthread_cond_wait(&cond, &lock);
		}
		pthread_mutex_unlock(&lock);
		printf("%d ", fib_seq[i]);
	}
	printf("\n");

	// Clear
	pthread_join(tid, NULL);
	free(fib_seq);
	return 0;
}

