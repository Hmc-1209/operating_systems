#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>

#define SHM_NAME "SHARED_MEMORY" 
#define BUFFER_SIZE sizeof(struct timeval)

int main(int argc, char *argv[]) {
	// Check program args 
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <command>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// Create shared memory
	int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
	if (shm_fd == -1) {
		perror("shm_open action failed!");
		exit(EXIT_FAILURE);
	}

	// Set shared memory size
	if (ftruncate(shm_fd, BUFFER_SIZE) == -1) {
		perror("ftruncate action failed!");
		exit(EXIT_FAILURE);
	}

	// Get space for shared memory
	struct timeval *start_time = mmap(0, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (start_time == MAP_FAILED) {
		perror("mmap action failed!");
		exit(EXIT_FAILURE);
	}

	// fork() the child process
	pid_t pid = fork();
	    
	// If pid goes error
	if (pid < 0) {
		perror("fork() action failed!");
		exit(EXIT_FAILURE);
	} else if (pid == 0) {
		// Child process
		gettimeofday(start_time, NULL);
		execlp(argv[1], argv[1], NULL);
		perror("'execlp' command execution failed!");
		exit(EXIT_FAILURE);
	} else {
		// Parent process
		wait(NULL);

		struct timeval end_time;
		gettimeofday(&end_time, NULL); 

		// Get elapsed time
		long seconds = end_time.tv_sec - start_time->tv_sec;
		long micros = end_time.tv_usec - start_time->tv_usec;
		double total_seconds = seconds + (micros / 1000000.0);
		printf("Elapsed time: %f\n", total_seconds);
			        
		// Delete shared memory
		munmap(start_time, BUFFER_SIZE);
		shm_unlink(SHM_NAME);
	}

	return 0;
}
