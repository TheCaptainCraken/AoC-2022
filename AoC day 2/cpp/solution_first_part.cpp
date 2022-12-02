#include <iostream>
#include <algorithm>
#include <fstream>
#include <bits/stdc++.h>

#define INPUT_FILE_PATH "input"

#define ROCK 'X'
#define PAPER 'Y'
#define SCISSORS 'Z'

#define ADVERSARY_ROCK 'A'
#define ADVERSARY_PAPER 'B'
#define ADVERSARY_SCISSORS 'C'

#define WIN 6
#define LOOSE 0
#define TIE 3

using namespace std;

int main(int argc, char* argv[]) {

	map<char, int> selection_scores = {
		{ROCK, 1},
		{PAPER, 2},
		{SCISSORS, 3}
	};

	map<pair<char, char>, int> match_scores = {
		{{ROCK, ADVERSARY_ROCK}, TIE},
		{{ROCK, ADVERSARY_PAPER}, LOOSE},
		{{ROCK, ADVERSARY_SCISSORS}, WIN},
		{{PAPER, ADVERSARY_ROCK}, WIN},
		{{PAPER, ADVERSARY_PAPER}, TIE},
		{{PAPER, ADVERSARY_SCISSORS}, LOOSE},
		{{SCISSORS, ADVERSARY_ROCK}, LOOSE},
		{{SCISSORS, ADVERSARY_PAPER}, WIN},
		{{SCISSORS, ADVERSARY_SCISSORS}, TIE}
	};

	ifstream input_file (INPUT_FILE_PATH);
	if(input_file.is_open()){
		string line;
		int score = 0;
		while(getline(input_file, line)){
			char me = line[2];
			char adversary = line[0];
			score+= selection_scores[me];
			score+= match_scores[{me, adversary}];
		}
		input_file.close();
		cout << score << endl;
	}else{
		cout << "Could not open the file you requested <" << INPUT_FILE_PATH << ">.";
	}

	return 0;
}