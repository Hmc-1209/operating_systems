#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

#define BUFFER_SIZE sizeof(struct timeval)

int main(int argc, char *argv[]) {
	// Check program args
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <command>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int pid_fd[2];
	pipe(pid_fd);

	// fork() for child process	
	pid_t pid = fork();

	// If pid goes error 
	if (pid < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	} else if (pid == 0) {
		// Child process
		struct timeval start_time;
		gettimeofday(&start_time, NULL);
		// Write current time to pipe
		write(pid_fd[1], &start_time, BUFFER_SIZE);
		execlp(argv[1], argv[1], NULL);
		perror("'execlp' command execute error!");
		exit(EXIT_FAILURE);
	} else {
		// Parent process
		wait(NULL);
		struct timeval start_time;
		
		// Read start time from pipe
		read(pid_fd[0], &start_time, BUFFER_SIZE);
		
		struct timeval end_time;
		gettimeofday(&end_time, NULL);
		
		long seconds = end_time.tv_sec - start_time.tv_sec;
		long micros = end_time.tv_usec - start_time.tv_usec;
		double total_seconds = seconds + (micros / 1000000.0);
		printf("Execution time: %f \n", total_seconds);
	}
}
