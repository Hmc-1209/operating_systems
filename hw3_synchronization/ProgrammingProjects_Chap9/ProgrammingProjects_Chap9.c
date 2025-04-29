#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct to represent memory section
typedef struct Block {
    	int start;
    	int size;
    	int allocated; // 0/1 = free/allocated
    	struct Block* next;
} Block;

Block* memory = NULL;
int MAX = 0;

// Allocate a section of free memory
void init_memory(int max_size) {
	memory = malloc(sizeof(Block));
	memory->start = 0;
	memory->size = max_size;
	memory->allocated = 0;
	memory->next = NULL;
}

// Requesting func
void request_memory(int size) {
    	Block* curr = memory;
    	while (curr) {
    		// Check if memory space enough
        	if (!curr->allocated && curr->size >= size) {
            		if (curr->size == size) {
                		// Directly allocate if size equal
                		curr->allocated = 1;
            		} else {
                		// Split into two section
                		Block* new_block = malloc(sizeof(Block));
                		new_block->start = curr->start + size;
                		new_block->size = curr->size - size;
                		new_block->allocated = 0;
                		new_block->next = curr->next;

                		curr->size = size;
                		curr->allocated = 1;
                		curr->next = new_block;
            		}
            		printf("Allocated at address %d\n", curr->start);
            		return;
        	}
        	curr = curr->next;
    	}
   	// If not enough space
    	printf("Request of size %d failed: Not enough space\n", size);
}

// Releasing func
void release_memory(int start) {
   	Block* curr = memory;
   	while (curr) {
       	 	// Find the starting point and free
        	if (curr->start == start && curr->allocated) {
            		curr->allocated = 0;
            		printf("Released memory at address %d\n", start);
            		return;
        	}
		// Go to next point
        	curr = curr->next;
    	}
 	// If not found starting point
    	printf("Release failed: No allocated block at address %d\n", start);
}

// Compact func
void compact_memory() {
    	Block* curr = memory;
    	Block* new_list = NULL, *last = NULL;
    	int next_start = 0;

    	// Move allocated section
    	while (curr) {
        	if (curr->allocated) {
            		Block* new_block = malloc(sizeof(Block));
            		new_block->start = next_start;
            		new_block->size = curr->size;
            		new_block->allocated = 1;
            		new_block->next = NULL;

            		if (!new_list) new_list = new_block;
            		else last->next = new_block;
            		last = new_block;
	
            		next_start += curr->size;
        	}
        	curr = curr->next;
    	}

    	// Add free section after allocation section
    	if (next_start < MAX) {
        	Block* free_block = malloc(sizeof(Block));
        	free_block->start = next_start;
        	free_block->size = MAX - next_start;
        	free_block->allocated = 0;
        	free_block->next = NULL;

        	if (!new_list) new_list = free_block;
        	else last->next = free_block;
    	}

    	// Release old linked-list
   	while (memory) {
        	Block* temp = memory;
        	memory = memory->next;
        	free(temp);
    	}

    	memory = new_list;
    	printf("Memory compacted.\n");
}

// Print func
void print_status() {
    	Block* curr = memory;
    	printf("Memory status:\n");
    	while (curr) {
        	printf("[%d - %d] : %s\n", curr->start, curr->start + curr->size - 1,
               		curr->allocated ? "Allocated" : "Free");
        	curr = curr->next;
    	}
}

int main(int argc, char* argv[]) {
    	// Check input argc
	if (argc != 2) {
        	printf("Usage: %s <memory_size>\n", argv[0]);
        	return 1;
    	}

	// Initial settings
    	MAX = atoi(argv[1]);
    	init_memory(MAX);
    	char line[100];
    	printf("Enter commands: REQUEST <size>, RELEASE <start>, COMPACT, STATUS, EXIT\n");

    	while (1) {
        	printf("> ");
        	fgets(line, sizeof(line), stdin);

        	char cmd[20];
        	int arg;
		
		// Switch mode
        	if (sscanf(line, "REQUEST %d", &arg) == 1) request_memory(arg);
        	else if (sscanf(line, "RELEASE %d", &arg) == 1) release_memory(arg);
        	else if (strncmp(line, "COMPACT", 7) == 0) compact_memory();
        	else if (strncmp(line, "STATUS", 6) == 0) print_status();
        	else if (strncmp(line, "EXIT", 4) == 0) break;
		else printf("Invalid command.\n");
    	}

    	return 0;
}
