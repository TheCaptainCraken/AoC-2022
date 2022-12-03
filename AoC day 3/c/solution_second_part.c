//usr/bin/gcc solution_second_part.c && exec ./a.out

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char** argv) {
	FILE* file = NULL;
	
	file = fopen("input", "r");
	if (file == NULL)
		exit(1);

	char line[3][256], index = 0, c;
	uint32_t score = 0;
	while (!feof(file)) {
		if (!fgets(line[index], sizeof line[0], file))
			break;

		++index;

		if (index > 2) {
			for (size_t i = 0; line[0][i] != 0; i++) {
				if (strchr(line[1], line[0][i]) != NULL && strchr(line[2], line[0][i]) != NULL) {
					score += line[0][i] - (line[0][i] > 'Z' ? 'a' : ('A' - 26)) + 1;
					break;
				}
			}
			
			index = 0;
		}
	}

	printf("%d\n", score);

	fclose(file);

	return 0;
}