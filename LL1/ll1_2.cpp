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

		void addRule(pair<char, string> rule) {
			rules.push_back(rule);
		}

		void createFirstAndFollow() {
		
			for (int i = 0; i < headerCol.size(); i++) {
				if (isupper(headerCol[i])) {
					set<char> tmp;
					first[headerCol[i]] = tmp;
					follow[headerCol[i]] = tmp;
				}
			}
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
		
		void fillFirst() {
			for (int i = 0; i < rules.size(); i++) {
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
						
						if (rules[i].second[0] == '~' && rules[m].second.length() > n + 1) {
							if (islower(rules[m].second[n + 1])) {
								first[ch1].insert(rules[m].second[n + 1]);
							} else {
								NonTerminalToNonTerminal(ch1, rules[m].second[n + 1], m , n + 1);
							}
						} else {
							first[ch1].insert(rules[i].second[0]);
						}
					} else if (isupper(rules[i].second[n])) {
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
									inserted = fillFollowRec(rules[i].second[j], i, j);
									changed = changed || inserted.second;
								}
							}
						}
					}
				}
				
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
			cout << ch1 << " " << ch2 << endl;
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
								fillFollowEnd(rules[i].second[j], rules[i].first, i);								
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
	}

	ll1.fillHeaders();
	ll1.createFirstAndFollow();
	
	ll1.fillFirst();

	ll1.fillFollow();

	cout << "First" << endl;
	ll1.showFirst();
	cout << endl << "Follow" << endl;
	ll1.showFollow();
	
	
	cout << endl;
	ll1.showMap();
	

/*
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
*/
	return 0;
}
