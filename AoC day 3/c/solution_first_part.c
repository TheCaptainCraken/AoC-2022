//usr/bin/gcc solution_first_part.c && exec ./a.out

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char** argv) {
	FILE* file = NULL;
	
	file = fopen("input", "r");
	if (file == NULL)
		exit(1);

	char line[256], found;
	uint32_t score = 0;
	while (!feof(file)) {
		if (!fgets(line, sizeof line, file))
			break;

		size_t len = strlen(line);
		found = 0;

		for (size_t i = 0; i < len / 2 && !found; i++)
			for (size_t j = len / 2; j < len && !found; j++)
				if (found = line[i] == line[j])
					score += line[i] - (line[i] > 'Z' ? 'a' : ('A' - 26)) + 1;
	}

	printf("%d\n", score);

	fclose(file);

	return 0;
}