#include <iostream>
#include <algorithm>
#include <bits/stdc++.h>

#define INPUT_FILE_PATH "input"

using namespace  std;

int main(int argc, char* argv[]) {

	int frequency_matrix[3][52] = {};

	string line;
	ifstream input_file(INPUT_FILE_PATH);
	if (input_file.is_open()) {
		int sum = 0;
		int count = 0;
		while (getline(input_file, line)) {
			int len = line.length();
			for (int i = 0; i < len; i++) {
				if (line[i] < 'a') {
					frequency_matrix[count][line[i] - 'A' + 26]++;
				} else {
					frequency_matrix[count][line[i] - 'a']++;
				}
			}
			count++;
			if (count == 3) {
				count = 0;
				for (int i = 0; i < 52; i++) {
					if (frequency_matrix[0][i] != 0 && frequency_matrix[1][i] != 0 && frequency_matrix[2][i] != 0)
						sum += i + 1;
					for (int c = 0; c < 3; c++) {
						frequency_matrix[c][i] = 0;
					}
				}
			}
		}
		input_file.close();

		cout << sum << endl;
	} else {
		cout << "Couldn't open the file!" << endl;
	}

	return 0;
}