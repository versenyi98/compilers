#include <iostream>

using namespace std;

class State {

	public:
		State(int id, int inputIndex, string input, int (&table)[20][14], int (&step)[20]) {

			this->input = input;
			this->id = id;
			this->inputIndex = inputIndex;

			this->table = &table;
			this->step = &step;


		}

	private:
		int id, inputIndex;
		string input;
		State* nextState;

		int (*table)[20][14];
		int (*step)[20];

		string logging() {

			switch(id) {
				case 2: return "Azonosító";
				case 4: return "Szám";
				case 7: return "{komment}";
				case 9: return "(*komment*)";
				case 12: return ":=token";
				case 14: return "<=token";
				case 15: return "<>token";
				case 17: return ">=token";
			}
		}
};

int main() {

	int table[20][14];
	
	int step[20];

	State state(0, 0, "xd", table, step);



	return 0;
};




