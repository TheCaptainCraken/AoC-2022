//usr/bin/gcc solution_second_part.c && exec ./a.out

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char** argv) {
	FILE *file = NULL;

	file = fopen("input", "r");
	if (file == NULL)
		exit(1);

	char temp[256];
	uint32_t counter = 0, read = 0;
	uint32_t max[3] = { 0, 0, 0 };

	while (!feof(file)) {
		fgets(temp, sizeof temp, file);

		if (sscanf(temp, "%u", &read) != -1) {
			counter += read;
		} else {
			// potrei fare qualcosa di più studiato ma 3 branch sono meglio di un sort successivo
			// potrei loopare su max ma GCC linearizzerebbe comunque i vari check perché l'index è basso
			if (counter > max[0]) {
				max[2] = max[1];
				max[1] = max[0];
				max[0] = counter;
			} else if (counter > max[1]) {
				max[2] = max[1];
				max[1] = counter;
			} else if (counter > max[2]) {
				max[2] = counter;
			}
			counter = 0;
		}
	}

	printf("%d\n", max[0] + max[1] + max[2]);
	
	return 0;
}
