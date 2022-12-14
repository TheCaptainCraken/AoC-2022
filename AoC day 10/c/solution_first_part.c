//usr/bin/gcc solution_first_part.c && exec ./a.out

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
	FILE *fp = fopen("input", "r");

	if (fp == NULL)
		return 1;

	uint64_t x = 1, addx = 0, clock_count = 1, strenght = 0;
	char op[5];

	while (!feof(fp)) {
		if (addx) {
			x += addx;
			addx = 0;
		} else {
			if (fscanf(fp, "%4s %lu", op, &addx) == -1)
				break;
		}

		clock_count++;

		if (clock_count > 20 && (clock_count - 20) % 40 == 0 || clock_count == 20) {
			printf("%lu %lu\n", clock_count, x);
			strenght += x * clock_count;
		}
	}

	printf("%lu\n", strenght);

	fclose(fp);

	return 0;
}