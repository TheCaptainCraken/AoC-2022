//usr/bin/gcc solution_first_part.c && exec ./a.out

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char** argv) {
	FILE *file = NULL;

	file = fopen("input", "r");
	if (file == NULL)
		exit(1);

	char temp[256];
	uint32_t counter = 0, max = 0, read = 0;

	while (!feof(file)) {
		fgets(temp, sizeof temp, file);

		if (sscanf(temp, "%u", &read) != -1) {
			counter += read;
		} else {
			if (counter > max)
				max = counter;
			counter = 0;
		}
	}

	printf("%d\n", max);
	
	return 0;
}
