//usr/bin/gcc solution_first_part.c && exec ./a.out

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MONKEYS 8
#define ROUND_COUNT 20

struct monkey {
	uint8_t id;
	uint32_t item_count;
	uint32_t items[64]; // too much?
	uint32_t inspection_count;
	char op_type; // '+' for sum, '*' for multiply
	uint32_t op_coeff; // 0 for "old", anything else as constant
	uint8_t test_criteria; // modulo
	uint8_t test_if_true;
	uint8_t test_if_false;
};

void parse_monkeys(struct monkey* m, FILE *fp)
{
	size_t index = 0;

	while (!feof(fp)) {
		uint8_t id = 0;
		fscanf(fp, "Monkey %hhu", &id);
		fgetc(fp); // skip newline

		m[index].id = id;

		size_t i = 0;
		fscanf(fp, "  Starting items: ");
		while (fscanf(fp, " %u", &m[index].items[i]) == 1) {
			fgetc(fp); // skip comma
			++i;
		}

		m[index].item_count = i;
		m[index].inspection_count = 0;

		fscanf(fp, "  Operation: new = old %c ", &m[index].op_type);
		char op_coeff[5];
		fgets(op_coeff, sizeof op_coeff, fp);
		op_coeff[4] = '\0';
		if (strcmp(op_coeff, "old)") == 0)
			m[index].op_coeff = 0;
		else
			m[index].op_coeff = atoi(op_coeff);

		fscanf(fp, "  Test: divisible by %hhu", &m[index].test_criteria);
		fgetc(fp); // skip newline

		fscanf(fp, "  If true: throw to monkey %hhu", &m[index].test_if_true);
		fgetc(fp); // skip newline

		fscanf(fp, "  If false: throw to monkey %hhu", &m[index].test_if_false);
		fgetc(fp); // skip newline

		fgetc(fp); // skip newline

		++index;
	}
}

void play_monkey_turn(struct monkey *m, size_t monkey_index)
{
	while (m[monkey_index].item_count) {
		++(m[monkey_index].inspection_count);

		uint32_t new_item = 0;
		
		if (m[monkey_index].op_type == '+')
			new_item = m[monkey_index].items[0] + m[monkey_index].op_coeff;
		else 
			new_item = m[monkey_index].items[0] * (m[monkey_index].op_coeff == 0 ? m[monkey_index].items[0] : m[monkey_index].op_coeff);

		new_item /= 3;

		if (new_item % m[monkey_index].test_criteria == 0) {
			m[m[monkey_index].test_if_true].items[m[m[monkey_index].test_if_true].item_count] = new_item;
			++(m[m[monkey_index].test_if_true].item_count);
		} else {
			m[m[monkey_index].test_if_false].items[m[m[monkey_index].test_if_false].item_count] = new_item;
			++(m[m[monkey_index].test_if_false].item_count);
		}

		--(m[monkey_index].item_count);
		for (size_t i = 0; i < m[monkey_index].item_count; ++i) {
			m[monkey_index].items[i] = m[monkey_index].items[i + 1];
		}
	}
}

void play_round(struct monkey *m)
{
	for (size_t i = 0; i < MAX_MONKEYS; i++)
		play_monkey_turn(m, i);
}

int main(int argc, char** argv) {
	FILE *fp;
	struct monkey monkeys[MAX_MONKEYS];
	
	fp = fopen("input", "r");
	if (fp == NULL)
		return 1;

	parse_monkeys(monkeys, fp);

	for (size_t i = 0; i < ROUND_COUNT; i++)
		play_round(monkeys);

	uint32_t max1, max2;
	max1 = max2 = 0;

	for (size_t i = 0; i < MAX_MONKEYS; ++i) {
		if (monkeys[i].inspection_count > max1) {
			max2 = max1;
			max1 = monkeys[i].inspection_count;
		} else if (monkeys[i].inspection_count > max2) {
			max2 = monkeys[i].inspection_count;
		}
	}

	printf("%u\n", max1 * max2);

	fclose(fp);

	return 0;
}
