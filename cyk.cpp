#include <iostream>
#include <vector>

using namespace std;

void createTableBody(string str, vector<vector<vector<char>>> &table, vector<pair<char, string>> rules);
void fillCells(vector<vector<vector<char>>> &table, vector<pair<char, string>> rules);
void viewTable(vector<vector<vector<char>>> table);

int main() {

	vector<vector<vector<char>>> table;
	vector<pair<char, string>> rules;

//	A szabályok megadása (elég alpha)
	rules.push_back(make_pair('S', "AB"));
	rules.push_back(make_pair('S', "CD"));
	rules.push_back(make_pair('S', "CB"));
	rules.push_back(make_pair('S', "SS"));
	rules.push_back(make_pair('A', "BC"));
	rules.push_back(make_pair('A', "a"));
	rules.push_back(make_pair('B', "SC"));
	rules.push_back(make_pair('B', "b"));
	rules.push_back(make_pair('C', "DD"));
	rules.push_back(make_pair('C', "b"));
	rules.push_back(make_pair('D', "BA"));

	createTableBody("aabbaba", table, rules);
	fillCells(table, rules);
	viewTable(table);

	return 0;
}

void createTableBody(string str, vector<vector<vector<char>>> &table, vector<pair<char, string>> rules) {

// 	A táblázatban a string hosszának megfelelő számú sornyi rombusz van
//	(+ 1 a string miatt)
	for(int i = 0; i < str.length() + 1; i++) {
		vector<vector<char>> row;
		table.push_back(row);
	}

//	Minden sorban a sor számának megfelelő számú rombusz van (1-gyel kezdve a számolást)
	for(int i = 0; i < str.length(); i++) {
		vector<char> rombusz;
		
		for(int j = 0; j <= i; j++) {
			table[i].push_back(rombusz);
		}	
	}

//	A legalsó sor feltöltése előbb rombuszokkal, majd azok feltölétse a megfelelő
//	karakterrel a sztringből. Az eggyel előbbi sor feltöltése, mivel itt más a szabály,
//	mint a többi sornál
	for(int i = 0; i <  str.length(); i++) {
		vector<char> rombusz;
		table[str.length()].push_back(rombusz);
		table[str.length()][i].push_back(str[i]);

		for(int j = 0; j < rules.size(); j++) {
			if(rules[j].second[0] == str[i]) {
				table[table.size() - 2][i].push_back(rules[j].first);
			}
		}

	}
}

void fillCells(vector<vector<vector<char>>> &table, vector<pair<char, string>> rules){

//	Az i. sor j. rombuszának értékeink meghatározása, az i,j pozíciójú rombusz meghatározásához
//	a k,j és k2,j2 rombusz értékeire van szükség, illetve a szabályok halmazára

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

void viewTable(vector<vector<vector<char>>> table) {

//	Az utolsó sor elejére 0 tab szükséges, minden más sor elejére eggyel többi mint az előzőben
	for(int i = 0; i < table.size() - 1; i++) {
		for(int l = 0; l < table.size() - 2 - i; l++){
			cout << '\t';
		}
	//	Egy tab a rombusz értékei előtt és egy utána, így középre igazítottnak tűnik, és el is lehet határolni "szépen"
		for(int j = 0; j < table[i].size(); j++) {
			cout << "|\t";
			for(int k = 0; k < table[i][j].size(); k++) {
				cout << table[i][j][k];
			}
			cout << "\t";
		}
		cout << "|\n";
	}

//	A string karaktereinek elhelyezése
	for(int i = 0; i < table[table.size() - 1].size(); i++) {
		cout << "|\t" << table[table.size() - 1][i][0] << "\t";
	}
	cout << "|\n";
}




