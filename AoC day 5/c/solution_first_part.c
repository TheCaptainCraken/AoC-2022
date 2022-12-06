//usr/bin/gcc solution_first_part.c && exec ./a.out

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define HEAD_COUNT 9

struct node {
	char value;
	struct node *next;
};

void parse_initial_state(FILE *fp, struct node *heads[HEAD_COUNT]);
void execute_moves(FILE *fp, struct node *heads[HEAD_COUNT]);
void print_result(struct node *heads[HEAD_COUNT]);

struct node *push_front(struct node *head, char value);
struct node *append(struct node *head, char value);
struct node *remove_first(struct node *head);
struct node *pop_last(struct node *head, char *value);
void cleanup_stack(struct node *head);

int main(int argc, char** argv)
{
	FILE *fp = NULL;
	struct node *heads[HEAD_COUNT] = { 0 };

	fp = fopen("input", "r");

	if (fp == NULL)
		return 1;

	parse_initial_state(fp, heads);
	execute_moves(fp, heads);
	print_result(heads);

	fclose(fp);

	return 0;
}

void parse_initial_state(FILE *fp, struct node *heads[HEAD_COUNT])
{
	char line[256] = { 0 };
	fgets(line, sizeof line, fp);

	do {
		for (int i = 0; i < HEAD_COUNT; i++)
			if (line[1 + i * 4] != ' ')
				heads[i] = push_front(heads[i], line[1 + i * 4]);

		fgets(line, sizeof line, fp);
	} while (line[1] != '1');
}

void execute_moves(FILE *fp, struct node *heads[HEAD_COUNT])
{
	char line[256] = { 0 };
	uint32_t count = 0, from = 0, to = 0;

	fgets(line, sizeof line, fp); //thow away empty line

	while (!feof(fp)) {
		if (!fgets(line, sizeof line, fp))
			break;
		
		if (sscanf(line, "move %u from %u to %u", &count, &from, &to) == -1)
			break;

		--from;
		--to;

		for (int i = 0; i < count; i++) {
			char last = 0;
			heads[from] = pop_last(heads[from], &last);
			heads[to] = append(heads[to], last);
		}
	}
}

void print_result(struct node *heads[HEAD_COUNT])
{
	for (int i = 0; i < HEAD_COUNT; i++) {
		char last = 0;
		heads[i] = pop_last(heads[i], &last);
		putchar(last);
		cleanup_stack(heads[i]);
	}
	putchar('\n');
}

struct node *push_front(struct node *head, char value)
{
	struct node *new_node = malloc(sizeof (struct node));
	new_node->value = value;
	new_node->next = head;
	return new_node;
}

struct node *append(struct node *head, char value)
{
	struct node *new_node = malloc(sizeof (struct node));
	new_node->value = value;
	new_node->next = NULL;

	if (head == NULL)
		return new_node;

	struct node *current = head;
	while (current->next != NULL)
		current = current->next;

	current->next = new_node;
	return head;
}

struct node *remove_first(struct node *head)
{
	struct node *new_head = head->next;
	free(head);
	return new_head;
}

struct node *pop_last(struct node *head, char *value)
{
	if (head->next == NULL) {
		*value = head->value;
		free(head);
		return NULL;
	}

	struct node *current = head;
	while (current->next->next != NULL)
		current = current->next;

	*value = current->next->value;
	free(current->next);
	current->next = NULL;
	return head;
}

void cleanup_stack(struct node *head)
{
	struct node *current = head;
	struct node *next = NULL;
	while (current != NULL) {
		next = current->next;
		free(current);
		current = next;
	}
}
