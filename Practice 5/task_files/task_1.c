#include <stdio.h>
#include <stdlib.h>

#define NUM_BLOCKS 10
#define BLOCK_SIZE (1024 * 1024)

int main() {
	char* blocks [NUM_BLOCKS];
		for (int i = 0; i < NUM_BLOCKS; ++i) {
			blocks[i] = malloc(BLOCK_SIZE);
				if (blocks[i] = NULL) {
				perror("malloc");
			return 1;
		}

	}

	printf("10 blocks of 1 MB allocated.\n");

	for (int i = 1; i < NUM_BLOCKS; i += 2) {
		free(blocks[i]);
	}

	printf("Every second block freed.\n");

	char* large_block = malloc(3 * BLOCK_SIZE);
	if (large_block == NULL) {
		printf("Failde to allocate 3 MB block due to fragmentation.\n");
	} else {
		printf("3 MB block allocated successfully.\n");
		free(large_block);
	}

	for (int i = 0; i < NUM_BLOCKS; i += 2) {
		free(blocks[i]);
	}

	return 0;
}
