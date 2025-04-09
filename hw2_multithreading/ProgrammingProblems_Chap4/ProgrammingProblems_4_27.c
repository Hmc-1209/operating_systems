#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int *fib_sequence;
int num;

void *fibonacci(void * arg) {
	if (num >= 1) fib_sequence[0] = 0;	// Set the first element
	if (num >= 2) fib_sequence[1] = 1;	// Set the second element

	// Calculate all fib numbers
	for (int i = 2; i < num; i++) fib_sequence[i] = fib_sequence[i - 1] + fib_sequence[i - 2];

	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	// Check input args
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <number>", argv[0]);
		return 1;
	}

	// Check input num
	num = atoi(argv[1]);
	if (num <= 0) {
		fprintf(stderr, "Please enter positive number.\n");
		return 1;
	}

	// Allocate memory
	fib_sequence = (int *)malloc(sizeof(int) * num);
	if (fib_sequence == NULL) {
		perror("Failed to allocate memory!");
		return 1;
	}

	// Create thread
	pthread_t tid;
	pthread_create(&tid, NULL, fibonacci, NULL);

	pthread_join(tid, NULL);

	// Print out the result
	printf("Fibnacci sqeuence:\n");
	for (int i = 0; i < num; i++) {
		printf("%d ", fib_sequence[i]);
	}
	printf("\n");

	// Free the memory and exit
	free(fib_sequence);
	return 0;
}
