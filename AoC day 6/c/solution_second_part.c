//usr/bin/gcc solution_second_part.c && exec ./a.out

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int is_unique_str(char *str) {
	for (size_t i = 0; str[i]; i++)
		for (size_t j = i + 1; str[j]; j++)
			if (str[i] == str[j])
				return 0;

	return 1;
}

int main(int argc, char** argv) {
	FILE *fp = NULL;

	fp = fopen("input", "r");

	if (fp == NULL)
		return 1;

	fseek(fp, 0, SEEK_END);

	size_t size = ftell(fp);

	fseek(fp, 0, SEEK_SET);

	char *buffer = calloc(size + 1, sizeof (char));
	char *prev_fourteen = buffer;
	size_t i = 0;

	for (; i < 14; i++, buffer++)
		*buffer = fgetc(fp);

	while (!feof(fp)) {
		*buffer = fgetc(fp);

		++buffer;
		++prev_fourteen;
		++i;

		if (is_unique_str(prev_fourteen)) {
			printf("%d\n", i);
			break;	
		}
	}

	fclose(fp);

	return 0;
}
