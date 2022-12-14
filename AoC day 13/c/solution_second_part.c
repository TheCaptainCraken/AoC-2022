//usr/bin/gcc solution_second_part.c && exec ./a.out

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define PACKET_COUNT 302

enum PACKET_TYPE {
	INTEGER = 0,
	LIST = 1
};

struct packet {
	enum PACKET_TYPE type;

	union content {
		uint32_t value;
		struct {
			size_t length;
			struct packet **elements;
		} list;
	} content;
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

struct packet *parse_packet(char *str)
{
	struct packet *packet = malloc(sizeof(struct packet));

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

int main(int argc, char** argv) {
	FILE *fp = fopen("input", "r");

	if (fp == NULL)
		return 1;

	size_t packet_count = 0;
	struct packet *packets[PACKET_COUNT];

	char line[512];
	while (!feof(fp)) {
		fgets(line, 512, fp);
		line[strlen(line) - 1] = '\0';
		packets[packet_count] = parse_packet(line);
		packet_count++;

		fgets(line, 512, fp);
		line[strlen(line) - 1] = '\0';
		packets[packet_count] = parse_packet(line);
		packet_count++;
		
		fgets(line, 512, fp); // skip empty line
	}

	fclose(fp);

	packets[packet_count] = parse_packet("[[2]]");
	packet_count++;
	packets[packet_count] = parse_packet("[[6]]");
	packet_count++;

	for (size_t i = 0; i < packet_count; i++) {
		for (size_t j = i + 1; j < packet_count; j++) {
			if (compare_element(packets[j], packets[i]) == 1) {
				struct packet *temp = packets[i];
				packets[i] = packets[j];
				packets[j] = temp;
			}
		}
	}

	struct packet *divider1 = parse_packet("[[2]]");
	struct packet *divider2 = parse_packet("[[6]]");

	size_t key = 1;

	for (size_t i = 0; i < packet_count; i++) {
		if (compare_element(packets[i], divider1) == 2) {
			key *= (i + 1);
		} else if (compare_element(packets[i], divider2) == 2) {
			key *= (i + 1);
		}
	}

	for (size_t i = 0; i < packet_count; i++)
		cleanup(packets[i]);

	cleanup(divider1);
	cleanup(divider2);

	printf("%zu\n", key);

	return 0;
}
