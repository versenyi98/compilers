#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <fstream>

using namespace std;

class LR0 {

	public:
		LR0() {};
		~LR0() {};
	
		void addRule(pair<char, string> rule) {
			rules.push_back(rule);
		}

		void fillCanonical() {
		
			fillLetters();
		
			pair<char, string> rule = rules[0];
			rule.second = "." + rule.second;
		
			set<pair<char, string>> closure_ = closure(rule);
			set<set<pair<char, string>>> canonicalTemp;
			canonicalTemp.insert(closure_);		
		
			bool changed = true;
			
			while (changed) {
			
				changed = false;
			
				for (auto i: canonicalTemp) {
					for (auto j: letters) {
						changed = canonicalTemp.insert(read(j, i)).second;	
						if (changed) break;				
					}
					if (changed) break;				
				}		
			}
			
			int index = 0;
			
			for (auto i: canonicalTemp) {	
				if (!i.empty()) {	
					canonical.push_back(i);
				}
			}
			
			fillTable();
		}

		void fillLetters() {
			
			for (auto i: rules) {
				letters.insert(i.first);
				for (auto j: i.second) {
					letters.insert(j);
				}
			}
		}

		void fillTable() {
		
			for (int i = 0; i < canonical.size(); i++) {
				for (auto j: letters) {
					table[j][i] = 0;
				}
			}
			
			for (auto i: letters) {
				for (int j = 0; j < canonical.size(); j++) {
				
					set<pair<char, string>> read_= read(i, canonical[j]);
				
					for (int k = 0; k < canonical.size(); k++) {
						if (read_ == canonical[k]) {
							table[i][j] = k + 1;
						}
					}
				}
			}
			fillAction();
		}

		void fillAction() {
			
			for (int i = 0; i < canonical.size(); i++) {
			
				bool step;

				for (auto j: canonical[i]) {
					
					step = j.second[j.second.size() - 1] != '.';
					if (!step) {
						action[i] = j.second.size() - 1;
						break;
					}
				}
				if (step) {
					action[i] = 0;
				}
			}
		}

		set<pair<char, string>> closure(pair<char, string> rule) {
				
			set<pair<char, string>> closure_;
			closure_.insert(rule);				
			
			bool changed = true;
			pair<set<pair<char, string>>::iterator, bool> inserted;

			while (changed) {
			
				changed = false;
			
				for (auto i : closure_) {
			
					int dot = -1;
					
					for (int j = 0; j < i.second.size(); j++) {
						if (i.second[j] == '.') {
							dot = j;
							break;
						}	
					}
					
					if (dot != i.second.size() - 1) {
						for (int j = 0; j < rules.size(); j++) {
							if (i.second[dot + 1] == rules[j].first) {
								inserted = closure_.insert(make_pair(rules[j].first, "." + rules[j].second));
								changed = changed || inserted.second;
							}
						}
					}		
				}
			}

			return closure_;
		}

		set<pair<char, string>> read(char X, set<pair<char, string>> closure_) {
		
			string pattern = ".";
			pattern += X;
		
			set<pair<char, string>> optimalRules;
			set<pair<char, string>> read_;
		
			for (auto i: closure_) {
				 size_t found = i.second.find(pattern);

  				if (found != string::npos) {
  				
  					i.second[found] = i.second[found + 1];
  					i.second[found + 1] = '.';
  					
  					optimalRules.insert(i);
  				}
			}
			
			for (auto i: optimalRules) {
				for (auto j: closure(i)) {
					read_.insert(j);
				}
			}
			
			return read_;
		}

		void parse(string input) {
		
			heap.second = input + "#";
			heap.first.push_back(make_pair('#', 1));
					
			while (true) {

				cout << heap.second << '\t';
				for(int i = 0; i < heap.first.size(); i++) {
					cout << heap.first[i].first << heap.first[i].second << " ";
				}
				cout << endl;				
				
				if (action[heap.first[heap.first.size() - 1].second - 1] == 0) {
				
					if (heap.first[heap.first.size() - 1].second == 1 && !heap.second.compare("A#")) {
						cout << "Megoldás" << endl;
						return;
					}

					if (heap.first[heap.first.size() - 1].second == 0) {
						cout << "Nemm egészen jó a szó." << endl;
						return;
					} 
				
					heap.first.push_back(make_pair(heap.second[0], table[heap.second[0]][heap.first[heap.first.size() - 1].second - 1]));
					heap.second = heap.second.substr(1);
					
				} else {
					
					char ch;
					for (auto i: canonical[heap.first[heap.first.size() - 1].second - 1]) {
						ch = i.first;
					}
					
					heap.second = ch + heap.second;
					int num = action[heap.first[heap.first.size() - 1].second - 1];
					for (int i = 0; i < num; i++) {
						heap.first.pop_back();
					}
				}
			}
		}

		void printClosure(set<pair<char, string>> closure) {
		
			for (auto i : closure) {
				cout << i.first << " " << i.second << endl;
			}
			cout << endl;
		}

		void printRules() {
			for (int i = 0; i < rules.size(); i++) {
				cout << rules[i].first << '\t' << rules[i].second << endl;
			}
		}

		void printCanonical() {
			
			int index = 1;
			
			for (auto i: canonical) {
				cout << index++ << ":" << endl;
				
				for (auto j: i) {
					cout << j.first << "\t" << j.second << endl;
				}
				cout << endl;
			}
		}

		void printTable() {
			
			for (auto i: letters) {
				cout << i << '\t';
			}
			cout << "\tAction" << endl;
			
			for (int j = 0; j < canonical.size(); j++) {
				for (auto i: letters) {
					cout << table[i][j] << '\t';
				}
				cout << j + 1 << '\t' << action[j] << endl;
			}
		}

	private:
		vector<pair<char, string>> rules;			
		vector<set<pair<char, string>>> canonical;
		set<char> letters;
				
		map<char, map<int, int>> table;
		map<int, int> action;				//0 -> léptetés, egyébként redukálás
		pair<vector<pair<char, int>>, string> heap;
	};

int main (int argv, char** args) {
		
	ifstream fin;

	if(argv > 1) {
		fin.open(args[1]);
	}

	if(!fin.is_open()) {
		cerr << "Használat: ./lr0 rules.txt" << endl;
		return -1;
	}
		
	LR0 lr0;
	
	string temp;
	while(getline(fin, temp)) {
		lr0.addRule(make_pair(temp[0], temp.substr(2)));
	}
	
	lr0.printRules();
	
	lr0.fillCanonical();
	lr0.printCanonical();
	lr0.printTable();

	string input;
	cout << "Adjon meg egy inputot: ";

	getline(cin, input);

	lr0.parse(input);

	return 0;
}
