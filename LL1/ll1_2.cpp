#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <fstream>
#include <set>

using namespace std;

class LL1 {

	public:

		LL1() {};
		~LL1() {};

	//Szabályok felvétele
		void addRule(pair<char, string> rule) {
			rules.push_back(rule);
		}

	//First és Follow map-ek létrehozása
		void createFirstAndFollow() {
			for (int i = 0; i < headerCol.size(); i++) {
				if (isupper(headerCol[i])) {
					set<char> tmp;
					first[headerCol[i]] = tmp;
					follow[headerCol[i]] = tmp;
				}
			}
		}
	
	//A header vectorok megöltése, col: nem terminális, row: nem terminális és terminális
		void fillHeaders() {
			for (int i = 0; i < rules.size(); i++) {
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

				for (int j = 0; j < rules[i].second.size(); j++) {
					bool init = false;

					for (int k = 0; k < headerCol.size(); k++) {
						if (headerCol[k] == rules[i].second[j]) {
							init = true;
							break;
						}
					}
					if (!init && !isupper(rules[i].second[j]) && rules[i].second[j] != '~') {
						headerCol.push_back(rules[i].second[j]);
						headerRow.push_back(rules[i].second[j]);
					}
				}
			}

			std::sort(headerCol.begin(), headerCol.end());
			std::sort(headerRow.begin(), headerRow.end());
		
			headerCol.push_back('#');
			headerRow.push_back('#');
		}
		
	//First megtöltése
		void fillFirst() {
			for (int i = 0; i < rules.size(); i++) {
			//Ha a szbály első eleme nem nagy akkor a first eleme
				if (!isupper(rules[i].second[0])) {
					first[rules[i].first].insert(rules[i].second[0]);
				}
			}
			
			for (int i = 0; i < rules.size(); i++) {
				if (isupper(rules[i].second[0])) {
					NonTerminalToNonTerminal(rules[i].first, rules[i].second[0], i, 0);
				}
			}
		}
		
		void NonTerminalToNonTerminal(char ch1, char ch2, int m, int n) {

			for (int i = 0; i < rules.size(); i++) {
				if (ch2 == rules[i].first) {

					if (!isupper(rules[i].second[0])) {
						//Ha lambda szabály tartozik a nem terminálishoz, akkor az eredeti szabály
						//következő karaktere adhat információt a first-ről
						if (rules[i].second[0] == '~' && rules[m].second.length() > n + 1) {
							if (islower(rules[m].second[n + 1])) {
								first[ch1].insert(rules[m].second[n + 1]);
							} else {
								NonTerminalToNonTerminal(ch1, rules[m].second[n + 1], m , n + 1);
							}
						} else {
							first[ch1].insert(rules[i].second[0]);
						}
					} else {
						NonTerminalToNonTerminal(ch1, rules[i].second[n], i, n);
					}
				}
			}
		}
				
		void fillFollow() {

			follow[rules[0].first].insert('#');

			pair<set<char>::iterator, bool> inserted;
			inserted.second = false;
			
			bool changed = true;
			
			while (changed) {
				changed = false;
				
				for (int i = 0; i < rules.size(); i++) {
					for (int j = 0; j < rules[i].second.size(); j++) {

						if (isupper(rules[i].second[j])) {
							if (j < rules[i].second.size() - 1) {
								if (!isupper(rules[i].second[j + 1])) {
									inserted = follow[rules[i].second[j]].insert(rules[i].second[j + 1]);
									changed = changed || inserted.second;
								} else {
									//Nem terminális follow-ja = a következő elem first-je
									inserted = fillFollowRec(rules[i].second[j], i, j);
									changed = changed || inserted.second;
								}
							}
						}
					}
				}
				
				//Szabály végén levő nem terminális followja = a bal oldal followja
				for (int i = 0; i < rules.size(); i++) {
					char ch = rules[i].second[rules[i].second.size() -1];
					if (isupper(ch)) {
						inserted = fillFollowEnd(ch, rules[i].first, i);
						changed = changed || inserted.second;
					}
				}

				for (int i = 0; i < rules.size(); i++) {
					if (rules[i].second[0] == '~') {
						inserted = fillFollowLambda(rules[i].first);
						changed = changed || inserted.second;
					}
				}
			}
		}
		
		pair<set<char>::iterator, bool> fillFollowRec(char ch, int i, int j) {
		
			pair<set<char>::iterator, bool> inserted;
			inserted.second = false;
			bool changed = false;
			
			set<char>::iterator it;

			for (it=first[rules[i].second[j + 1]].begin(); it!=first[rules[i].second[j + 1]].end(); ++it) {
				if (*it != '~') {
					inserted = follow[ch].insert(*it);
					changed = changed || inserted.second;
				} else {
					if (j + 2 < rules[i].second.length()) {
						fillFollowRec(ch, i, j + 1);
					} else {
						fillFollowEnd(ch, rules[i].first, i);
					}
				}
			}
			inserted.second = changed;

			return inserted;
		}
		
		pair<set<char>::iterator, bool> fillFollowEnd(char ch1, char ch2, int i) {

			pair<set<char>::iterator, bool> inserted;
			inserted.second = false;
			bool changed = false;

			if (ch1 == ch2) {
				return inserted;
			}

			set<char>::iterator it;
			for (it=follow[ch2].begin(); it!=follow[ch2].end(); ++it) {

				inserted = follow[ch1].insert(*it);
				changed = changed || inserted.second;
			}
			
			inserted.second = changed;
			return inserted;
		}
		
		pair<set<char>::iterator, bool> fillFollowLambda(char ch) {
		
			pair<set<char>::iterator, bool> inserted;
			inserted.second = false;
			bool changed = false;
		
			set<char>::iterator it;
		
			for (int i = 0; i < rules.size(); i++) {
				for (int j = 0; j < rules[i].second.size() - 1; j++) {
					if (isupper(rules[i].second[j]) && rules[i].second[j + 1] == ch) {
						for (it=first[rules[i].second[j + 1]].begin(); it!=first[rules[i].second[j + 1]].end(); ++it) {
						
							if (*it == '~') {
								fillFollowRec(rules[i].second[j], i, j + 1);
								cout << "Meghívódik" << endl;								
							} else {
								inserted = follow[rules[i].second[j]].insert(*it);
								changed = changed || inserted.second;	
							}
						}
					}
				}
			}
		
			inserted.second = changed;
			return inserted;
		}
		
		void fillTable() {
		
			for (int i = 0; i < headerCol.size(); i++) {
				for (int j = 0; j < headerRow.size(); j++) {
					table[headerCol[i]][headerRow[j]] = 0;
					if (headerCol[i] == headerRow[j]) {
						table[headerCol[i]][headerRow[j]] = -1;
						if (headerCol[i] == '#') {
							table[headerCol[i]][headerRow[j]] = -2;
						}
					}
				}
			}
			
			for (int i = 0; i < rules.size(); i++) {
				fillTableRec(i, 0);
			}		
		}
		
		void fillTableRec(int i, int j) {
			if (rules[i].second[j] == '~') {
				set<char>::iterator it;
				for (it=follow[rules[i].first].begin(); it!=follow[rules[i].first].end(); ++it) {
					table[rules[i].first][*it] = i + 1;
				}
			
			} else if (isupper(rules[i].second[j])) {
				
				set<char>::iterator it;
				for (it=first[rules[i].second[j]].begin(); it!=first[rules[i].second[j]].end(); ++it) {
					if (*it == '~') {
						fillTableRec(i, j + 1);
					} else {
						table[rules[i].first][*it] = i + 1; 
					}
				}
			} else {
				table[rules[i].first][rules[i].second[j]] = i + 1;
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

		void showFirst() {
		
			set<char>::iterator it;
		
			for (auto i : first){
				cout << i.first << ":\t";
				
				for (it=i.second.begin(); it!=i.second.end(); ++it) {
					cout << *it << " ";
				}
				cout << endl;
			}
		}
	
		void showFollow() {
		
			set<char>::iterator it;
		
			for (auto i : follow){
				cout << i.first << ":\t";
				
				for (it=i.second.begin(); it!=i.second.end(); ++it) {
					cout << *it << " ";
				}
				cout << endl;
			}
		}
	
		void parse(string input) {
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
		map<char, set<char>> first;
		map<char, set<char>> follow;
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
		cout << temp << endl;
	}

	cout << endl;

	ll1.fillHeaders();
	ll1.createFirstAndFollow();
	
	ll1.fillFirst();

	ll1.fillFollow();

	cout << "First" << endl;
	ll1.showFirst();
	cout << endl << "Follow" << endl;
	ll1.showFollow();
	
	ll1.fillTable();
	
	cout << endl;
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
