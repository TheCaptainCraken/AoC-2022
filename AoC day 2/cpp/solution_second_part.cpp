#include <iostream>
#include <algorithm>
#include <fstream>
#include <bits/stdc++.h>

#define INPUT_FILE_PATH "input"

#define LOOSE 'X'
#define TIE 'Y'
#define WIN 'Z'

#define ROCK 'A'
#define PAPER 'B'
#define SCISSORS 'C'

using namespace std;

int main(int argc, char* argv[]) {

	map<char, int> selection_scores = {
		{ROCK, 1},
		{PAPER, 2},
		{SCISSORS, 3}
	};

	map<char, int> match_scores = {
		{LOOSE, 0},
		{TIE, 3},
		{WIN, 6}
	};

	map<pair<char, char>, char> cheat_sheet = {
		{{LOOSE, ROCK}, SCISSORS},
		{{LOOSE, PAPER}, ROCK},
		{{LOOSE, SCISSORS}, PAPER},
		{{TIE, PAPER}, PAPER},
		{{TIE, ROCK}, ROCK},
		{{TIE, SCISSORS}, SCISSORS},
		{{WIN, SCISSORS}, ROCK},
		{{WIN, PAPER}, SCISSORS},
		{{WIN, ROCK}, PAPER}
	};

	ifstream input_file (INPUT_FILE_PATH);
	if(input_file.is_open()){
		string line;
		int score = 0;
		while(getline(input_file, line)){
			char outcome = line[2];
			char adversary = line[0];
			
			score+= match_scores[outcome];
			score += selection_scores[cheat_sheet[{outcome, adversary}]];
		}
		input_file.close();
		cout << score << endl;
	}else{
		cout << "Could not open the file you requested <" << INPUT_FILE_PATH << ">.";
	}

	return 0;
}