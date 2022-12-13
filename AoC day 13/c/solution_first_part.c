//usr/bin/gcc solution_first_part.c && exec ./a.out

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

enum PACKET_TYPE {
	INTEGER = 0,
	LIST = 1
};

struct packet {
	enum PACKET_TYPE type;

	union {
		uint32_t value;
		struct {
			size_t length;
			struct packet **elements;
		} list;
	} content;
};

struct packet_pair {
	size_t index;
	struct packet *left, *right;
};

size_t first_delimiting_comma(char str[512])
{
	size_t depth = 0;
	for (size_t i = 0; i < strlen(str); i++) {
		if (str[i] == '[') {
			depth++;
		} else if (str[i] == ']') {
			depth--;
		} else if (str[i] == ',' && depth == 0) {
			return i;
		}
	}

	return 0;
} 

struct packet *parse_packet(char str[512])
{
	struct packet *packet = malloc(sizeof(struct packet));

	// printf("Parsing packet: %s\n", str);

	if (str[0] == '[') {
		packet->type = LIST;
		packet->content.list.length = 0;
		packet->content.list.elements = NULL;

		size_t comma_index = 0;
		while (comma_index = first_delimiting_comma(str + 1)) {
			str[comma_index + 1] = '\0';

			packet->content.list.length++;
			packet->content.list.elements = realloc(packet->content.list.elements, packet->content.list.length * sizeof(struct packet*));
			packet->content.list.elements[packet->content.list.length - 1] = parse_packet(str + 1);
			str += comma_index + 1;

			// printf("str: %s, comma_index: %d\n", str, comma_index);
		}

		// parse last element (if there is one)
		if (strlen(str + 1) > 1 && str[1] != ']') {
			packet->content.list.length++;
			packet->content.list.elements = realloc(packet->content.list.elements, packet->content.list.length * sizeof(struct packet*));
			packet->content.list.elements[packet->content.list.length - 1] = parse_packet(str + 1);
		}

		return packet;
	} else {
		packet->type = INTEGER;
		packet->content.value = atoi(str);
		return packet;
	}
}

struct packet_pair *parse_packet_pair(FILE *fp)
{
	struct packet_pair *packet_pair = malloc(sizeof(struct packet_pair));

	char str[512];
	fgets(str, sizeof str, fp);
	str[strlen(str) - 1] = '\0';

	packet_pair->left = parse_packet(str);

	fgets(str, sizeof str, fp);
	str[strlen(str) - 1] = '\0';

	packet_pair->right = parse_packet(str);

	fgetc(fp); // skip newline

	return packet_pair;
}

void print_packet(struct packet *packet)
{
	if (packet->type == INTEGER) {
		printf("%d", packet->content.value);
	} else {
		printf("[");
		for (size_t i = 0; i < packet->content.list.length; i++) {
			print_packet(packet->content.list.elements[i]);
			if (i != packet->content.list.length - 1)
				printf(",");
		}
		printf("]");
	}
}

void cleanup(struct packet *packet)
{
	if (packet->type == LIST) {
		for (size_t i = 0; i < packet->content.list.length; i++) {
			cleanup(packet->content.list.elements[i]);
		}
		free(packet->content.list.elements);
	}

	free(packet);
}

void cleanup_pair(struct packet_pair *packet_pair)
{
	cleanup(packet_pair->left);
	cleanup(packet_pair->right);
	free(packet_pair);
}

uint8_t compare_element(struct packet *left, struct packet *right)
{
	if (left->type == INTEGER && right->type == INTEGER) {
		if (left->content.value < right->content.value)
			return 1;
		else if (left->content.value == right->content.value)
			return 2;
		else
			return 0;
	} else if (left->type == LIST && right->type == LIST) {
		size_t index = 0;

		while (1) {
			if (index >= left->content.list.length && index >= right->content.list.length)
				return 2;
			else if (index >= left->content.list.length)
				return 1;
			else if (index >= right->content.list.length)
				return 0;

			uint8_t result = compare_element(left->content.list.elements[index], right->content.list.elements[index]);

			if (result != 2)
				return result;

			index++;
		}
	} else if (left->type == INTEGER && right->type == LIST) {
		struct packet *templist = malloc(sizeof(struct packet));
		templist->type = LIST;
		templist->content.list.length = 1;
		templist->content.list.elements = malloc(sizeof(struct packet*));
		templist->content.list.elements[0] = left;

		uint8_t result = compare_element(templist, right);

		free(templist->content.list.elements);
		free(templist);

		return result;
	} else if (left->type == LIST && right->type == INTEGER) {
		struct packet *templist = malloc(sizeof(struct packet));
		templist->type = LIST;
		templist->content.list.length = 1;
		templist->content.list.elements = malloc(sizeof(struct packet*));
		templist->content.list.elements[0] = right;

		uint8_t result = compare_element(left, templist);

		free(templist->content.list.elements);
		free(templist);

		return result;
	}
}

size_t compare_pair(struct packet_pair *pair)
{
	uint8_t res = compare_element(pair->left, pair->right);

	assert(res != 2);

	if (res == 1)
		return pair->index;
	else
		return 0;
}

int main(int argc, char** argv) {
	FILE *fp = fopen("input", "r");

	if (fp == NULL)
		return 1;

	size_t packet_count = 1;
	struct packet_pair **packet_pairs = malloc(packet_count * sizeof(struct packet_pair*));

	while (!feof(fp)) {
		packet_pairs[packet_count - 1] = parse_packet_pair(fp);
		packet_pairs[packet_count - 1]->index = packet_count;
		packet_count++;
		packet_pairs = realloc(packet_pairs, packet_count * sizeof(struct packet_pair*));
	}

	fclose(fp);

	// for (size_t i = 0; i < packet_count - 1; i++) {
	// 	printf("Left:  ");
	// 	print_packet(packet_pairs[i]->left);
	// 	printf("\nRight: ");
	// 	print_packet(packet_pairs[i]->right);
	// 	printf("\n\n");
	// }

	uint64_t sum = 0;

	for (size_t i = 0; i < packet_count - 1; i++) {
		uint64_t result = compare_pair(packet_pairs[i]);

		sum += result;
	}

	for (size_t i = 0; i < packet_count - 1; i++)
		cleanup_pair(packet_pairs[i]);

	printf("Sum: %llu\n", sum);

	free(packet_pairs);

	return 0;
}