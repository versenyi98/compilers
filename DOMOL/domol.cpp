#include <iostream>
#include <ctype.h>

using namespace std;

class State {

	public:
		State(int id, int inputIndex, string input, int (&table)[20][14], int (&step)[20]) {

			this->input = input;
			this->id = id;
			this->inputIndex = inputIndex;

			this->table = &table;
			this->step = &step;

			this->nextState = nullptr;

//			cout << input << " " << inputIndex << " "<< input[inputIndex] << " " << id << endl;

			if(inputIndex <= input.size()) {
				setNextState();
			}
		}

		~State() { delete nextState; }

		void logging() {

			switch(id) {
				case 2: cout << "Azonosító "; break;
				case 4: cout << "Szám "; break;
				case 6: cout << "{komment} "; break;
				case 10: cout << "(*komment*) "; break;
				case 12: cout << ":=token "; break;
				case 14: cout << "<=token "; break;
				case 15: cout << "<>token "; break;
				case 17: cout << ">=token "; break;
				case 18: cout << "Hibakezelő "; break;
				case 19: cout << "Továbbfejl. "; break;
				case 20: cout << "stop "; break;
				default: cout << ""; break;
			}
		
			if(nextState != nullptr) {
				nextState->logging();
			}
		}

	private:
		int id, inputIndex;
		string input;
		State* nextState;

		int (*table)[20][14];
		int (*step)[20];

		void setNextState() {
			if(inputIndex == input.length()) {
				return;
			}

			if(isalpha(input[inputIndex])) {
				nextState = new State((*table)[id][0] - 1, inputIndex + (*step)[id], input, *table, *step);
			} else if (isdigit(input[inputIndex])) {
				nextState = new State((*table)[id][1] - 1, inputIndex + (*step)[id], input, *table, *step);
			} else if (input[inputIndex] == '{') {
				nextState = new State((*table)[id][2] - 1, inputIndex + (*step)[id], input, *table, *step);
			} else if (input[inputIndex] == '}') {
				nextState = new State((*table)[id][3] - 1, inputIndex + (*step)[id], input, *table, *step);
			} else if (input[inputIndex] == '(') {
				nextState = new State((*table)[id][4] - 1, inputIndex + (*step)[id], input, *table, *step);
			} else if (input[inputIndex] == '*') {
				nextState = new State((*table)[id][5] - 1, inputIndex + (*step)[id], input, *table, *step);
			} else if (input[inputIndex] == ')') {
				nextState = new State((*table)[id][6] - 1, inputIndex + (*step)[id], input, *table, *step);
			} else if (input[inputIndex] == ':') {
				nextState = new State((*table)[id][7] - 1, inputIndex + (*step)[id], input, *table, *step);
			} else if (input[inputIndex] == '=') {
				nextState = new State((*table)[id][8] - 1, inputIndex + (*step)[id], input, *table, *step);
			} else if (input[inputIndex] == '<') {
				nextState = new State((*table)[id][9] - 1, inputIndex + (*step)[id], input, *table, *step);
			} else if (input[inputIndex] == '>') {
				nextState = new State((*table)[id][10] - 1, inputIndex + (*step)[id], input, *table, *step);
			} else if (isspace(input[inputIndex])) {
				nextState = new State((*table)[id][11] - 1, inputIndex + (*step)[id], input, *table, *step);
			} else if (input[inputIndex] == '$') {
				nextState = new State((*table)[id][13] - 1, inputIndex + (*step)[id], input, *table, *step);
			} else {
				nextState = new State((*table)[id][12] - 1, inputIndex + (*step)[id], input, *table, *step);
			}

		}
};

int main(int argv, char** args) {

	int table[20][14]	= 
	 {{ 2,	 4,	 6,	19,	 8,	19,	19,	12,	19,	14,	17,	 1,	19,	21},
		{ 2,	 2,	 3,	 3,	 3,	 3,	 3,	 3,	 3,	 3,	 3,	 3,	 3,	 3},
		{ 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1},
		{ 5,	 4,	 5,	 5,	 5,	 5,	 5,	 5,	 5,	 5,	 5,	 5,	 5,	 5},
		{ 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1},
		{ 6,	 6,	 6,	 7,	 6,	 6,	 6,	 6,	 6,	 6,	 6,	 6,	 6,	 6},
		{ 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1},
		{20,	20,	20,	20,	20,	 9,	20,	20,	20,	20,	20,	20,	20,	20},
		{ 9,	 9,	 9,	 9,	 9,	10,	 9,	 9,	 9,	 9,	 9,	 9,	 9,	 9},
		{ 9,	 9,	 9,	 9,	 9,	10,	11,	 9,	 9,	 9,	 9,	 9,	 9,	 9},
		{ 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1},
		{20,	20,	20,	20,	20,	20,	20,	20,	13,	20,	20,	20,	20,	20},
		{ 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1},
		{20,	20,	20,	20,	20,	20,	20,	20,	15,	20,	16,	20,	20,	20},
		{ 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1},
		{ 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1},
		{20,	20,	20,	20,	20,	20,	20,	20,	18,	20,	20,	20,	20,	20},
		{ 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1},
		{ 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1},
		{ 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1,	 1}};

	int step[20] = 
		{ 1, 1, -1, 1,-1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0,-1};

	while(true) {
		cout << "Adj meg valami inputot:\n";

		string input;
		getline(cin, input);
		input += ' ';

		State state(0, 0, input, table, step);

		state.logging();

		cout << endl << endl;
	}

	return 0;
};
