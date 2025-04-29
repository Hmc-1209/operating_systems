#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>  // for uint32_t

#define PAGE_SIZE 4096
#define OFFSET_BITS 12

int main(int argc, char *argv[]) {
	// Check input argc
    	if (argc != 2) {
       		fprintf(stderr, "Usage: %s <virtual_address>\n", argv[0]);
        	return 1;
   	}

    	uint32_t virtual_address = (uint32_t)atoi(argv[1]);

	// Seperate page and offset from virtual address
    	uint32_t page_number = virtual_address >> OFFSET_BITS;     // Higher 20 bits
    	uint32_t offset = virtual_address & 0xFFF;                 // Lower 12 bits

    	printf("The address %u contains:\n", virtual_address);
    	printf("page number = %u\n", page_number);
    	printf("offset = %u\n", offset);

    	return 0;
}

