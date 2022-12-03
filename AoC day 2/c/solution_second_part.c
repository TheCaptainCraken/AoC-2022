//usr/bin/gcc solution_second_part.c && exec ./a.out

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char** argv) {
	FILE *fp = NULL;
	
	fp = fopen("input", "r");
	if (fp == NULL)
		exit(1);

	char line[8], first, second, diff;
	uint32_t score = 0;
	while (!feof(fp)) {
		if (!fgets(line, sizeof line, fp))
			break;

		first = line[0] - 'A';
		second = line[2] - 'X';

		second = (first + second + 2) % 3;

		diff = (3 + second - first) % 3;

		if (diff == 0) {
			score += 3;
		} else if (diff == 1) {
			score += 6;
		}

		score += second + 1;
	}

	printf("%d\n", score);
}
