#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int main() {
	char targetFile[256] = "target.txt";
    	char destFile[256] = "destination.txt";
	
	// Open target file
	int targetFD = open(targetFile, O_RDONLY);
    	if (targetFD < 0) {
        	perror("Cannot open target file!");
       		return EXIT_FAILURE;
    	}

	// Create destination file
	int destFD = open(destFile, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
   	if (destFD < 0) {
        	perror("Cannot create destination file!");
        	close(targetFD);
        	return EXIT_FAILURE;
    	}

	// Copy target file content to destination file
	char buffer[BUFFER_SIZE];
    	ssize_t bytesRead;
    	while ((bytesRead = read(targetFD, buffer, BUFFER_SIZE)) > 0) {
        	if (write(destFD, buffer, bytesRead) != bytesRead) {
            		perror("Error while writing to destination file!");
            		close(targetFD);
           		close(destFD);
            		return EXIT_FAILURE;
        	}
    	}

	// Check file bytes
	if (bytesRead < 0) {
        	perror("File read error!");
    	}

    	// Close and terminate program
    	close(targetFD);
    	close(destFD);

    	printf("File copy success！\n");
    	return EXIT_SUCCESS;
}
