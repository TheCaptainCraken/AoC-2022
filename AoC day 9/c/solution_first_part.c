//usr/bin/gcc solution_first_part.c && exec ./a.out

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct pos {
	int32_t x;
	int32_t y;
};

struct game_data {
	struct pos head_pos;
	struct pos tail_pos;

	size_t unique_tail_pos_count;
	struct pos *unique_tail_pos;
};

void add_unique_tail_pos(struct game_data *game_data) {
	struct pos pos = game_data->tail_pos;

	for (size_t i = 0; i < game_data->unique_tail_pos_count; i++) {
		if (game_data->unique_tail_pos[i].x == pos.x && game_data->unique_tail_pos[i].y == pos.y)
			return;
	}

	game_data->unique_tail_pos_count++;
	game_data->unique_tail_pos = realloc(game_data->unique_tail_pos, game_data->unique_tail_pos_count * sizeof(struct pos));
	game_data->unique_tail_pos[game_data->unique_tail_pos_count - 1] = pos;
}

void move_tail(struct game_data *game_data) {
	int32_t diff_x = game_data->head_pos.x - game_data->tail_pos.x;
	int32_t diff_y = game_data->head_pos.y - game_data->tail_pos.y;

	// Otherwise, if the head and tail aren't touching and aren't in the same row or column, the tail always moves one step diagonally to keep up:

	if (abs(diff_x) > 1 && abs(diff_y)) {
		game_data->tail_pos.x += diff_x / abs(diff_x);
		game_data->tail_pos.y += diff_y / abs(diff_y);
	} else if (abs(diff_y) > 1 && abs(diff_x)) {
		game_data->tail_pos.x += diff_x / abs(diff_x);
		game_data->tail_pos.y += diff_y / abs(diff_y);
	} else if (abs(diff_x) > 1) {
		game_data->tail_pos.x += diff_x / abs(diff_x);
	} else if (abs(diff_y) > 1) {
		game_data->tail_pos.y += diff_y / abs(diff_y);
	}

	add_unique_tail_pos(game_data);
}

void move_head(struct game_data *game_data, char direction) {
	switch (direction) {
		case 'D':
			game_data->head_pos.y--;
			break;
		case 'U':
			game_data->head_pos.y++;
			break;
		case 'L':
			game_data->head_pos.x--;
			break;
		case 'R':
			game_data->head_pos.x++;
			break;
	}
}

int main(int argc, char** argv) {
	FILE *fp = fopen("input", "r");

	if (fp == NULL)
		return 1;

	struct game_data game_data = { .head_pos = {0, 0}, .tail_pos = {0, 0}, .unique_tail_pos_count = 0, .unique_tail_pos = NULL };

	while (!feof(fp)) {
		char direction;
		uint32_t count;
		if (fscanf(fp, "%c %u", &direction, &count) != 2)
			break;

		fgetc(fp);

		for (uint32_t i = 0; i < count; i++) {
			move_head(&game_data, direction);
			move_tail(&game_data);
		}

		printf("%c %u: (%d, %d) (%d, %d)\n", direction, count, game_data.head_pos.x, game_data.head_pos.y, game_data.tail_pos.x, game_data.tail_pos.y);
	}

	printf("%u\n", game_data.unique_tail_pos_count);

	free(game_data.unique_tail_pos);

	fclose(fp);

	return 0;
}