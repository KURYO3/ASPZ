#include <stdio.h>
#include <stdlib.h>

int main() {
	int *arr = malloc(5 * sizeof(int));

	for (int i = 0; i < 5; ++i) {
		arr[i] = i * 10;
	}

	int val = arr[4];

	printf("Read value: %d\n", val);

	free(arr);
	return 0;
}