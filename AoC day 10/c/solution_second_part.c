//usr/bin/gcc solution_second_part.c && exec ./a.out

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
	FILE *fp = fopen("input", "r");

	if (fp == NULL)
		return 1;

	int64_t x = 1, addx = 0;
	uint64_t clock_count = 0;
	char op[5];

	while (!feof(fp)) {
		if (addx) {
			x += addx;
			addx = 0;
		} else {
			if (fscanf(fp, "%4s %ld", op, &addx) == -1)
				break;
		}

		clock_count++;

		if (abs(x - (clock_count % 40)) <= 1) {
			printf("#");
		} else {
			printf(".");
		}
		if (clock_count % 40 == 0)
			printf("\n");
	}

	fclose(fp);

	return 0;
}