//usr/bin/gcc solution_second_part.c && exec ./a.out

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define DIM 512
#define XDIM 1024

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

char world[DIM][XDIM];

void fill_world()
{
	for (uint16_t i = 0; i < DIM; i++)
		for (uint16_t j = 0; j < XDIM; j++)
			world[i][j] = '.';
}

void parse_world(FILE *file)
{
	uint16_t x, y, nx, ny, maxy = 0;

	while (!feof(file)) {
		if (fscanf(file, "%hu,%hu", &x, &y) != 2)
			break;

		fgetc(file); // skip space

		while (fscanf(file, "->") != -1) {
			fscanf(file, " %hu,%hu", &nx, &ny);

			for (uint16_t i = min(x, nx); i <= max(x, nx); i++)
				for (uint16_t j = min(y, ny); j <= max(y, ny); j++)
					world[j][i] = '#';

			x = nx;
			y = ny;

			maxy = max(maxy, ny);

			if (fgetc(file) == '\n')
				break;
		}
	}

	for (uint16_t i = 0; i < XDIM; i++)
		world[maxy + 2][i] = '#';
}

void print_world()
{
	FILE *out = fopen("world", "w");

	if (out == NULL)
		return;

	for (uint16_t i = 0; i < DIM; i++) {
		for (uint16_t j = 0; j < XDIM; j++)
			fprintf(out, "%c", world[i][j]);

		fprintf(out, "\n");
	}

	fclose(out);
}

uint8_t add_sand()
{
	uint16_t x = 500, y = 0, nx, ny;

	if (world[y][x] != '.')
		return 0;

	world[y][x] = 'o';

	while (1) {
		if (world[y + 1][x] == '.') {
			ny = y + 1;
			nx = x;
		} else if (x > 0 && world[y + 1][x - 1] == '.') {
			ny = y + 1;
			nx = x - 1;
		} else if (x < (XDIM - 1) && world[y + 1][x + 1] == '.') {
			ny = y + 1;
			nx = x + 1;
		} else {
			break;
		}

		world[y][x] = '.';
		world[ny][nx] = 'o';
		x = nx;
		y = ny;
	}

	return 1;
}

int main(int argc, char** argv) {
	FILE *fp = fopen("input", "r");

	if (fp == NULL)
		return 1;

	fill_world();
	parse_world(fp);

	uint32_t count = 0;

	while (add_sand())
		++count;

	print_world();

	printf("count: %d\n", count);

	fclose(fp);

	return 0;
}
