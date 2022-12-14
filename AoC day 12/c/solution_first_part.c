//usr/bin/gcc solution_first_part.c && exec ./a.out

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define COLNUM 64
#define ROWNUM 41

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

char grid[ROWNUM][COLNUM]; // today we use globals just for fun

struct pos {
	uint8_t x, y;
};

struct node {
	uint8_t is_start_pos : 1;
	uint8_t is_end_pos : 1;
	uint8_t visited : 1;
	uint16_t distance;
	struct pos node_pos;
	uint8_t neighbour_count;
	struct pos neighbours[4];
};

struct node nodes[ROWNUM][COLNUM] = {0}; // today we use globals just for fun

void parse_grid(FILE *fp)
{
	char line[COLNUM + 1];
	int i = 0;
	while (fscanf(fp, "%64s", line) != -1) {
		fgetc(fp); // skip newline
		memcpy(grid[i], line, COLNUM);
		i++;
	}
}

void parse_nodes(struct pos *start, struct pos *end)
{
	for (int i = 0; i < ROWNUM; i++) {
		for (int j = 0; j < COLNUM; j++) {
			if (grid[i][j] == 'S') {
				start->x = j;
				start->y = i;
				nodes[i][j].is_start_pos = 1;
				grid[i][j] = 'a';
			} else if (grid[i][j] == 'E') {
				end->x = j;
				end->y = i;
				nodes[i][j].is_end_pos = 1;
				grid[i][j] = 'z';
			} else {
				nodes[i][j].is_start_pos = 0;
				nodes[i][j].is_end_pos = 0;
			}
		}
	}

	for (int i = 0; i < ROWNUM; i++) {
		for (int j = 0; j < COLNUM; j++) {
			nodes[i][j].node_pos.x = j;
			nodes[i][j].node_pos.y = i;
			nodes[i][j].visited = 0;
			nodes[i][j].distance = UINT16_MAX;

			nodes[i][j].neighbour_count = 0;

			// I hate this, I'm sure there's a better way, I'm sorry
			if (i > 0 && (grid[i][j] + 1) >= grid[i - 1][j]) {
				nodes[i][j].neighbours[nodes[i][j].neighbour_count].x = j;
				nodes[i][j].neighbours[nodes[i][j].neighbour_count].y = i - 1;
				nodes[i][j].neighbour_count++;
			}
			if (i < ROWNUM - 1 && (grid[i][j] + 1) >= grid[i + 1][j]) {
				nodes[i][j].neighbours[nodes[i][j].neighbour_count].x = j;
				nodes[i][j].neighbours[nodes[i][j].neighbour_count].y = i + 1;
				nodes[i][j].neighbour_count++;
			}
			if (j > 0 && (grid[i][j] + 1) >= grid[i][j - 1]) {
				nodes[i][j].neighbours[nodes[i][j].neighbour_count].x = j - 1;
				nodes[i][j].neighbours[nodes[i][j].neighbour_count].y = i;
				nodes[i][j].neighbour_count++;
			}
			if (j < COLNUM - 1 && (grid[i][j] + 1) >= grid[i][j + 1]) {
				nodes[i][j].neighbours[nodes[i][j].neighbour_count].x = j + 1;
				nodes[i][j].neighbours[nodes[i][j].neighbour_count].y = i;
				nodes[i][j].neighbour_count++;
			}
		}
	}
}

int main(int argc, char** argv) {
	FILE *fp = fopen("input", "r");

	if (fp == NULL)
		return 1;

	parse_grid(fp);

	fclose(fp);

	struct pos start, end, current;

	parse_nodes(&start, &end);

	nodes[start.y][start.x].distance = 0;

	current = start;

	while (1) {
		nodes[current.y][current.x].visited = 1;

		for (int i = 0; i < nodes[current.y][current.x].neighbour_count; i++) {
			struct pos neighbour = nodes[current.y][current.x].neighbours[i];
			if (nodes[neighbour.y][neighbour.x].visited)
				continue;

			nodes[neighbour.y][neighbour.x].distance = min(nodes[neighbour.y][neighbour.x].distance, nodes[current.y][current.x].distance + 1);
		}

		uint16_t min_distance = UINT16_MAX;
		for (int i = 0; i < ROWNUM; i++) {
			for (int j = 0; j < COLNUM; j++) {
				if (nodes[i][j].visited)
					continue;

				if (nodes[i][j].distance < min_distance) {
					min_distance = nodes[i][j].distance;
					current.x = j;
					current.y = i;
				}
			}
		}

		if (min_distance == UINT16_MAX) {
			printf("no path found\n");
			break;
		}

		if (nodes[current.y][current.x].is_end_pos)
			break;
	}

	printf("%d\n", nodes[end.y][end.x].distance);

	return 0;
}
