//usr/bin/gcc solution_second_part.c && exec ./a.out

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
	FILE *fp = NULL;

	fp = fopen("input", "r");
	if (fp == NULL)
		return 1;

	char line[256];
	uint8_t a, b, c, d;
	uint32_t count = 0;

	while (!feof(fp)) {
		if (!fgets(line, sizeof line, fp))
			break;

		if (sscanf(line, "%hhu-%hhu,%hhu-%hhu", &a, &b, &c, &d) != 4)
			break;

		if (a <= c && b >= c || a <= d && b >= d || c <= a && d >= a || c <= b && d >= b)
			++count;
	}

	printf("%u\n", count);

	return 0;
}
