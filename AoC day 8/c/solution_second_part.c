//usr/bin/gcc solution_second_part.c && exec ./a.out

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define N 99

uint8_t grid[N][N] = {0};
uint32_t scenic_score[N][N];

int main(int argc, char** argv) {
	FILE *fp = fopen("input", "r");
	
	if (fp == NULL)
		return 1;

	for (size_t i = 0; i < N; i++) {
		for (size_t j = 0; j < N; j++) {
			grid[i][j] = (uint8_t) (getc(fp) - '0');
		}
		getc(fp); // newline
	}

	for (size_t i = 0; i < N; i++)
		for(size_t j = 0; j < N; j++)
			scenic_score[i][j] = 1;

	int8_t last_pos[10];
	
	for (ssize_t i = 0; i < N; i++) {
		for (size_t k = 0; k < 10; k++)
			last_pos[k] = 0;
		for (ssize_t j = 0; j < N; j++) {
			int8_t digit = (int8_t) grid[i][j];
			int8_t score = abs((int8_t) j - last_pos[digit]);
			scenic_score[i][j] *= score;
			for (size_t k = 0; k <= digit; k++)
				last_pos[k] = j;
		}
	}

	for (ssize_t i = 0; i < N; i++) {
		for (size_t k = 0; k < 10; k++)
			last_pos[k] = N - 1;
		for (ssize_t j = N - 1; j >= 0; j--) {
			int8_t digit = (int8_t) grid[i][j];
			int8_t score = abs((int8_t) j - last_pos[digit]);
			scenic_score[i][j] *= score;
			for (size_t k = 0; k <= digit; k++)
				last_pos[k] = j;
		}
	}

	for (ssize_t j = 0; j < N; j++) {
		for (size_t k = 0; k < 10; k++)
			last_pos[k] = 0;
		for (ssize_t i = 0; i < N; i++) {
			int8_t digit = (int8_t) grid[i][j];
			int8_t score = abs((int8_t) i - last_pos[digit]);
			scenic_score[i][j] *= score;
			for (size_t k = 0; k <= digit; k++)
				last_pos[k] = i;
		}
	}

	for (ssize_t j = 0; j < N; j++) {
		for (size_t k = 0; k < 10; k++)
			last_pos[k] = N - 1;
		for (ssize_t i = N - 1; i >= 0; i--) {
			int8_t digit = (int8_t) grid[i][j];
			int8_t score = abs((int8_t) i - last_pos[digit]);
			scenic_score[i][j] *= score;
			for (size_t k = 0; k <= digit; k++)
				last_pos[k] = i;
		}
	}
	

	uint32_t max = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (scenic_score[i][j] > max)
				max = scenic_score[i][j];

	printf("%d\n", max);

	fclose(fp);

	return 0;
}