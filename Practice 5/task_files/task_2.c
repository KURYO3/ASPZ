#include <stdio.h>
#include <stdlib.h>

#define NUM_BLOCKS 90000
#define BLOCK_SIZE 2048

int main() {
	char* blocks [NUM_BLOCKS];
		for (int i = 0; i < NUM_BLOCKS; ++i) {
			blocks[i] = malloc(BLOCK_SIZE);
			if (blocks[i] == NULL) {
				printf("malloc failed at block %d\n", i);
				return 1;
			}

		}

	printf("Allocated ad blocks of %d bytes each.\n", NUM_BLOCKS, BLOCK_SIZE);

	for (int i = 0; i < NUM_BLOCKS; i += 3) {
		free(blocks[i]);
	}

	printf("Freed every third block to create fragmentation.\n");

	char* big = malloc(30 * 1024 * 1024);
	if (!big) {
		printf("Failde to allocate large 30 MB block due to fragmentation.\n");
	} else {
		printf("Large 30 MB block allocated successfully.\n");
		free(big);
	}

	for (int i = 0; i < NUM_BLOCKS; ++i) {
			if (i % 3 != 0)
				free(blocks[i]);
	}

	return 0;
}