//usr/bin/gcc solution_first_part.c && exec ./a.out

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define N 99

int8_t grid[N][N] = {0};
uint8_t visited[N][N] = {0};

int main(int argc, char** argv) {
	FILE *fp = fopen("input", "r");
	
	if (fp == NULL)
		return 1;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			grid[i][j] = (int8_t) (getc(fp) - '0');
		}
		getc(fp); // newline
	}

	int8_t min;

	for (int i = 0; i < N; i++) {
		min = -1;
		for (int j = 0; j < N; j++) {
			if (grid[i][j] > min) {
				min = grid[i][j];
				visited[i][j] = 1;
			}
		}
	}

	for (int i = 0; i < N; i++) {
		min = -1;
		for (int j = N - 1; j >= 0; j--) {
			if (grid[i][j] > min) {
				min = grid[i][j];
				visited[i][j] = 1;
			}
		}
	}

	for (int j = 0; j < N; j++) {
		min = -1;
		for (int i = 0; i < N; i++) {
			if (grid[i][j] > min) {
				min = grid[i][j];
				visited[i][j] = 1;
			}
		}
	}

	for (int j = 0; j < N; j++) {
		min = -1;
		for (int i = N - 1; i >= 0; i--) {
			if (grid[i][j] > min) {
				min = grid[i][j];
				visited[i][j] = 1;
			}
		}
	}

	int32_t count = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (visited[i][j])
				count++;

	printf("%d\n", count);

	fclose(fp);

	return 0;
}