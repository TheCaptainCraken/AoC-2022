//usr/bin/gcc solution_second_part.c && exec ./a.out

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 256

struct fs_node {
	char *name;
	size_t size;
	size_t subnode_count;
	struct fs_node *parent;
	struct fs_node **subnodes;
};

struct fs_node_list {
	struct fs_node *node;
	struct fs_node_list *next;
};

struct fs_node *create_node(char *name, size_t size, struct fs_node *parent);
struct fs_node *append_child_to_node(struct fs_node *node, struct fs_node *child);
struct fs_node *navigate_to(struct fs_node *root, char *path);
struct fs_node_list* filter_nodes(struct fs_node *node, uint8_t (*filter)(struct fs_node *node), size_t *matching_count);
struct fs_node_list* list_append_back(struct fs_node_list *list, struct fs_node *node);
struct fs_node_list* list_concatenate(struct fs_node_list *list, struct fs_node_list *other);
void compute_sizes(struct fs_node *node);
size_t compute_size(struct fs_node *node);
void cleanup_node(struct fs_node *node);
void cleanup_list(struct fs_node_list *list);

void parse_input(struct fs_node *root, FILE *fp);
char* parse_listing(char *line, size_t *size);
char *parse_cd(char *line);
inline int is_command(char *line);
inline int is_cd(char *line);
inline int is_ls(char *line);

uint8_t select_for_size(struct fs_node *node)
{
	return node->subnode_count;
}

int main(int argc, char** argv) {
	struct fs_node *root = malloc(sizeof(struct fs_node));

	FILE *fp = fopen("input", "r");
	if (fp == NULL)
		return -1;

	parse_input(root, fp);

	compute_sizes(root);

	size_t matching_count = 0;

	struct fs_node_list *list = filter_nodes(root, select_for_size, &matching_count);

	printf("Found %zu matching nodes:\n", matching_count);

	size_t needed_size = 30000000 - (70000000 - root->size);
	size_t found_size = root->size;

	for (struct fs_node_list *dir = list; dir; dir = dir->next) {
		if (dir->node->size >= needed_size && dir->node->size < found_size)
			found_size = dir->node->size;
	}

	printf("Found size: %zu\n", found_size);

	cleanup_node(root);

	fclose(fp);

	return 0;
}

struct fs_node *create_node(char *name, size_t size, struct fs_node *parent)
{
	struct fs_node *node = malloc(sizeof(struct fs_node));
	node->name = name;
	node->size = size;
	node->subnode_count = 0;
	node->parent = parent;
	node->subnodes = NULL;

	return node;
}

struct fs_node *append_child_to_node(struct fs_node *node, struct fs_node *child)
{
	node->subnode_count++;
	if (!node->subnodes) {
		node->subnodes = malloc(sizeof(struct fs_node *));
	} else {
		node->subnodes = realloc(node->subnodes, sizeof(struct fs_node *) * node->subnode_count);
	}
	node->subnodes[node->subnode_count - 1] = child;

	return node;
}

struct fs_node *navigate_to(struct fs_node *root, char *path)
{
	if (!strcmp(path, "..")) {
		return root->parent;
	}

	for (size_t i = 0; i < root->subnode_count; i++) {
		if (!strcmp(root->subnodes[i]->name, path)) {
			return root->subnodes[i];
		}
	}
}

struct fs_node_list* filter_nodes(struct fs_node *node, uint8_t (*filter)(struct fs_node *node), size_t *matching_count)
{
	struct fs_node_list *list = NULL;

	if (filter(node)) {
		list = list_append_back(list, node);
		(*matching_count)++;
	}

	for (size_t i = 0; i < node->subnode_count; i++) {
		list = list_concatenate(list, filter_nodes(node->subnodes[i], filter, matching_count));
	}

	return list;
}

struct fs_node_list* list_append_back(struct fs_node_list *list, struct fs_node *node)
{
	struct fs_node_list *new_node = malloc(sizeof(struct fs_node_list));
	new_node->node = node;
	new_node->next = NULL;

	if (!list) {
		return new_node;
	}

	while (list->next) {
		list = list->next;
	}

	list->next = new_node;
}

struct fs_node_list* list_concatenate(struct fs_node_list *list, struct fs_node_list *other)
{
	if (!list) {
		return other;
	}

	struct fs_node_list *last = list;
	while (last->next) {
		last = last->next;
	}

	last->next = other;

	return list;
}

void compute_sizes(struct fs_node *node)
{
	if (node->subnode_count == 0) {
		return;
	}

	for (size_t i = 0; i < node->subnode_count; i++) {
		compute_sizes(node->subnodes[i]);
	}

	node->size = compute_size(node);
}

size_t compute_size(struct fs_node *node)
{
	if (node->subnode_count == 0) {
		return node->size;
	}

	size_t size = 0;
	for (size_t i = 0; i < node->subnode_count; i++) {
		size += compute_size(node->subnodes[i]);
	}

	return size;
}

void cleanup_node(struct fs_node *node)
{
	for (size_t i = 0; i < node->subnode_count; i++) {
		cleanup_node(node->subnodes[i]);
	}

	free(node->name);
	free(node->subnodes);
	free(node);
}

void cleanup_list(struct fs_node_list *list)
{
	if (list->next) {
		cleanup_list(list->next);
	}

	free(list);
}

void parse_input(struct fs_node *root, FILE *fp)
{
	char line[256] = { };

	struct fs_node *current_dir = root;

	uint8_t is_listing = 0;

	// setup root node
	fgets(line, sizeof(line), fp);
	char *name = parse_cd(line);
	current_dir->name = name;
	current_dir->size = 0;
	current_dir->subnode_count = 0;
	current_dir->parent = NULL;
	current_dir->subnodes = NULL;

	while (!feof(fp)) {
		if (!fgets(line, sizeof(line), fp))
			break;

		if (!is_command(line) && !is_listing) {
			printf("Something went wrong while parsing the input: did not expect %s\n", line);
			return;
		} else if (is_cd(line)) {
			is_listing = 0;
			char *name = parse_cd(line);
			current_dir = navigate_to(current_dir, name);
		} else if (is_ls(line)) {
			is_listing = 1;
		} else if (is_listing) {
			size_t size = 0;
			char *name = parse_listing(line, &size);
			struct fs_node *new = create_node(name, size, current_dir);
			current_dir = append_child_to_node(current_dir, new);
		} else {
			printf("Something went wrong while parsing the input: did not expect %s\n", line);
			return;
		}
	}
}

char* parse_listing(char *line, size_t *size)
{
	char *name = malloc(sizeof(char) * MAX_NAME_LENGTH);

	if (line[0] == 'd') {
		sscanf(line, "dir %s", name);
	} else {
		sscanf(line, "%lu %s", size, name);
	}

	return name;
}

char *parse_cd(char *line)
{
	char *name = malloc(sizeof(char) * MAX_NAME_LENGTH);
	sscanf(line, "$ cd %s", name);

	return name;
}

int is_command(char *line)
{
	return line[0] == '$';
}

int is_cd(char *line)
{
	return is_command(line) && line[2] == 'c' && line[3] == 'd';
}

int is_ls(char *line)
{
	return is_command(line) && line[2] == 'l' && line[3] == 's';
}
