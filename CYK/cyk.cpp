#include <iostream>
#include <vector>
#include <ctype.h>
#include <fstream>

using namespace std;

class CYK {
	public:
		CYK() {
			table.clear();
		}

		CYK(string word) {
			table.clear();

			this->word = word;
		}

		~CYK() {}

		void createTableBody() {
			table.clear();
		// 	A táblázatban a string hosszának megfelelő számú sornyi rombusz van
		//	(+ 1 a string miatt)
			for(int i = 0; i < word.length() + 1; i++) {
				vector<vector<char>> row;
				table.push_back(row);
			}

		//	Minden sorban a sor számának megfelelő számú rombusz van (1-gyel kezdve a számolást)
			for(int i = 0; i < word.length(); i++) {
				vector<char> rombusz;
				
				for(int j = 0; j <= i; j++) {
					table[i].push_back(rombusz);
				}	
			}

		//	A legalsó sor feltöltése előbb rombuszokkal, majd azok feltölétse a megfelelő
		//	karakterrel a sztringből. Az eggyel előbbi sor feltöltése, mivel itt más a szabály,
		//	mint a többi sornál
			for(int i = 0; i <  word.length(); i++) {
				vector<char> rombusz;
				table[word.length()].push_back(rombusz);
				table[word.length()][i].push_back(word[i]);

					if(isupper(word[i])) {
						table[table.size() - 2][i].push_back('S');
						continue;
					}

				for(int j = 0; j < rules.size(); j++) {
					if(rules[j].second[0] == word[i]) {
						table[table.size() - 2][i].push_back(rules[j].first);
					}
				}
			}
		}

		void fillCells() {
			for(int i = table.size() - 1; i >= 0; i--) {
				for(int j = 0; j < table[i].size(); j++) {
					int counter = 0;
					for(int k = i + 1; k < table.size()- 1; k++) {
		//			cout << i << "," << j << ": " << k << "," << j << " : " << table.size() - k - 1 + i << "," << table.size() - k - 1 + j << endl;

						int k2 = table.size() - k - 1 + i;
						int j2 = table.size() - k - 1 + j;

						for(int l = 0; l < rules.size(); l++) {
							for(int m = 0; m < table[k][j].size(); m++) {
								for(int n = 0; n < table[k2][j2].size(); n++) {
									if(!rules[l].second.compare(string(1, table[k][j][m]) + string(1, table[k2][j2][n]))) {
										table[i][j].push_back(rules[l].first);
									}
								}
							}
						}
					}
				}
			}
		}

		void viewTable() {

		//	Az utolsó sor elejére 0 tab szükséges, minden más sor elejére eggyel többi mint az előzőben
			for(int i = 0; i < table.size() - 1; i++) {
				for(int l = 0; l < table.size() - 2 - i; l++){
					cout << '\t';
				}
				
		//	A Rublikák határolója
				cout << ' ';
				for(int j = 0; j < table[i].size(); j++) {
					cout << "_______________ ";
					
				}
				cout << endl;

				for(int l = 0; l < table.size() - 2 - i; l++){
					cout << '\t';
				}

			//	Egy tab a rombusz értékei előtt és egy utána, így középre igazítottnak tűnik, és el is lehet határolni "szépen"
				for(int j = 0; j < table[i].size(); j++) {
					cout << "|\t";
					for(int k = 0; k < table[i][j].size(); k++) {
						cout << table[i][j][k];
					}
					cout << '\t';
				}
				cout << "|\n";
			}

		//	A string karaktereinek elhelyezése
			for(int i = 0; i < table[table.size() - 1].size(); i++) {
				cout << "|\t" << table[table.size() - 1][i][0] << "\t";
			}
			cout << "|\n";
		}

		void setWord(string word) {
			this->word = word;
		}

		void addRule(pair<char, string> rule) {
			rules.push_back(rule);
		}

		bool valid() {
			if(table.size() && table[0][0].size()) {
				for(int i = 0; i < table[0][0].size(); i++) {
					if(table[0][0][i] == 'S') {
						return true;
					}
				}
			}

			return false;
		}

	private:
		vector<vector<vector<char>>> table;
		vector<pair<char, string>> rules;

		string word;
};

int main(int argv, char** args) {

	ifstream fin;

	if(argv > 1) {
		fin.open(args[1]);
	}

	if(!fin.is_open()) {
		cerr << "Használat: ./cyk rules.txt" << endl;
		return -1;
	}

	string input;
			
	cout << "Írj be egy inputot: \n";
	cin >> input;

	CYK cyk(input);
	string temp;
	while(getline(fin, temp)) {
		cyk.addRule(make_pair(temp[0], temp.substr(2)));
	}
	
	cyk.createTableBody();
	cyk.fillCells();
	cyk.viewTable();
	cout << input << " szó eleme a nyelvnek? " << cyk.valid() << endl;

	return 0;
}

