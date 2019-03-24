#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <fstream>

using namespace std;

class LL1 {

	public:

		LL1() {};
		~LL1() {};

		void addRule(pair<char, string> rule) {
			rules.push_back(rule);
		}

		void fillHeaders() {
			for (int i = 0; i < rules.size(); i++) {
				//Szabályok bal oldáról az egyedi nagybetűk összegyűjtése
				bool init = false;

				for (int j = 0; j < headerCol.size(); j++) {
					if (headerCol[j] == rules[i].first) {
						init = true;
						break;
					}
				}

				if (!init) {
					headerCol.push_back(rules[i].first);
				}

				//Szabályok jobb oldaláról az egyedi kisbetűk összegyűjtése
				for (int j = 0; j < rules[i].second.size(); j++) {
					bool init = false;

					for (int k = 0; k < headerCol.size(); k++) {
						if (headerCol[k] == rules[i].second[j]) {
							init = true;
							break;
						}
					}
					if (!init && !isupper(rules[i].second[j])) {
						headerCol.push_back(rules[i].second[j]);
						headerRow.push_back(rules[i].second[j]);
					}
				}
			}

			//Rendezés, hogy nézzen ki valahogy a sorrend
			std::sort(headerCol.begin(), headerCol.end());
			std::sort(headerRow.begin(), headerRow.end());
		
			//# karakter is a táblázatban kell hogy szerepeljen
			headerCol.push_back('#');
			headerRow.push_back('#');
		}

		void fillTable() {
			//Táblázat feltöltése:
				// 0 	- error
				// -1 - pop
				// -2 - accept
				// >0	-	szabály alkalmazása

			for (int i = 0; i < headerCol.size(); i++) {
				for (int j = 0; j < headerRow.size(); j++) {
					
					table[headerCol[i]][headerRow[j]] = 0;

					for (int k = 0; k < rules.size(); k++) {
						if (headerCol[i] == rules[k].first && headerRow[j] == rules[k].second[0] && !isupper(rules[k].second[0]) && rules[k].second[0] != '~') {
							table[headerCol[i]][headerRow[j]] = k + 1;
						}
					}

					if (headerCol[i] == headerRow[j]) {
						table[headerCol[i]][headerRow[j]] = -1;
						if (headerCol[i] == '#') {
							table[headerCol[i]][headerRow[j]] = -2;
						}
					} 
				}
			}
			
			// N -> N (N or T)* szabályok
			for (int i = 0; i < rules.size(); i++) {
				if (isupper(rules[i].second[0])) {
					NonTerminalToNonTerminal(rules[i].first, rules[i].second[0], i + 1);
				}
			}
			
			for (int i = 0; i < rules.size(); i++) {
				if(rules[i].second[0] == '~') {
					handleLambda(rules[i].first, i + 1);
				}
			}
		}
		
		void NonTerminalToNonTerminal(char ch1, char ch2, int num) {

			for (int i = 0; i < rules.size(); i++) {
				if (ch2 == rules[i].first) {
					if (islower(rules[i].second[0])) {
						table[ch1][rules[i].second[0]] = num; 
					} else if (isupper(rules[i].second[0])) {
						NonTerminalToNonTerminal(ch1, rules[i].second[0], num);
					} else if (rules[i].second[0] == '~') {
						cout << "lambda" << endl;
						if (islower(rules[num - 1].second[1])) {
							cout << ch1 << '\t' << rules[num - 1].second[1] << endl;
							table[ch1][rules[num - 1].second[1]] = num; 
						} else {
							NonTerminalToNonTerminal(ch1, rules[num - 1].second[1], num);
						}
					}
				}
			}
		}
		
		void handleLambda(char ch, int num) {
			for (int i = 0; i < rules.size(); i++) {
				for (int j = 0; j < rules[i].second.size(); j++) {
					if (rules[i].second[j] == ch && islower(rules[i].second[j + 1])) {
						table[ch][rules[i].second[j + 1]] = num;
					} else if (rules[i].second[j] == ch && !islower(rules[i].second[j + 1])) {
						NonTerminalToNonTerminal(ch, rules[i].second[j + 1], num);
					}
				}
			}
		}

		void showMap() {
			for (int i = 0; i < headerRow.size(); i++) {
				cout << headerRow[i] << '\t';
			}
		 
			cout << endl;

			for (int i = 0; i < headerCol.size(); i++) {

				for (int j = 0; j < headerRow.size(); j++) {
					cout << table[headerCol[i]][headerRow[j]] << '\t';
				}
				cout << headerCol[i] << endl;
			}
			cout << endl;
		}

		void parse(string input) {
		//Elemzés
			input += "#";
			string state = "S#";
			vector<int> rulesUsed;

			while (true) {

				cout << input << ",\t" << state << ",\t(";
				for (int i = 0; i < rulesUsed.size(); i++) {
					cout << rulesUsed[i] << ' ';
				}
				cout << ")\n";

				int number = table[state[0]][input[0]];

				if (number > 0) {
					if (rules[number - 1].second[0] != '~') {
						state = rules[number - 1].second + state.substr(1);
					} else {
						state = state.substr(1);
					}
					rulesUsed.push_back(number);
					
				} else if (number == -1) {
					input = input.substr(1);
					state = state.substr(1);
				} else if (number == -2) {
					cout << "A szó eleme a nyelvnek..." << endl;
					break;
				} else {
					cout << "Error" << endl;
					break;
				}
			}
		}

	private:
		vector<pair<char, string>> rules;
		map<char, map<char, int>> table;
		vector<char> headerRow, headerCol;
};

int main(int argv, char** args) {

	ifstream fin;

	if(argv > 1) {
		fin.open(args[1]);
	}

	if(!fin.is_open()) {
		cerr << "Használat: ./ll1 rulesLL1.txt" << endl;
		return -1;
	}

	LL1 ll1;

	string temp;
	while(getline(fin, temp)) {
		ll1.addRule(make_pair(temp[0], temp.substr(2)));
	}

	ll1.fillHeaders();
	ll1.fillTable();
	ll1.showMap();

	string input;

	cout << "Adjon meg egy inputot: ";

	while (getline(cin, input)) {

		if (input.length() == 0) {
			break;
		}
		cout << endl;
		ll1.parse(input);

		cout << endl << "Adjon meg egy inputot: ";
	}

	cout << endl;

	return 0;
}

