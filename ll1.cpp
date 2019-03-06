#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

int main(int argv, char** args) {

//A táblázat betűinek kiszűrése
	vector<pair<char, string>> rules;
	
	rules.push_back(make_pair('S', "aS"));
	rules.push_back(make_pair('S', "bAc"));
	rules.push_back(make_pair('A', "bAc"));
	rules.push_back(make_pair('A', "d"));

	vector<char> small, both;
	
	for (int i = 0; i < rules.size(); i++) {

		bool init = false;

		for (int j = 0; j < both.size(); j++) {
			if (both[j] == rules[i].first) {
				init = true;
				break;
			}
		}

		if (!init) {
			both.push_back(rules[i].first);
		}

		for (int j = 0; j < rules[i].second.size(); j++) {
			bool init = false;

			for (int k = 0; k < both.size(); k++) {
				if (both[k] == rules[i].second[j]) {
					init = true;
					break;
				}
			}
			if (!init && islower(rules[i].second[j])) {
				both.push_back(rules[i].second[j]);
				small.push_back(rules[i].second[j]);
			}
		}
	}

	std::sort(both.begin(), both.end());
	std::sort(small.begin(), small.end());

	both.push_back('#');
	small.push_back('#');

//Map feltöltése
	map<char, map<char, int>> tableMap;


	for (int i = 0; i < both.size(); i++) {
		for (int j = 0; j < small.size(); j++) {
			
			tableMap[both[i]][small[j]] = 0;

			for (int k = 0; k < rules.size(); k++) {
				if (both[i] == rules[k].first && small[j] == rules[k].second[0]) {
					tableMap[both[i]][small[j]] = k + 1;
				}
			}

			if (both[i] == small[j]) {

				tableMap[both[i]][small[j]] = -1;

				if (both[i] == '#') {
					tableMap[both[i]][small[j]] = -2;
				}
			} 
		}
	}

//Map kirajzolása
	for (int i = 0; i < small.size(); i++) {
		cout << small[i] << '\t';
	}
 
	cout << endl;

	for (int i = 0; i < both.size(); i++) {

		for (int j = 0; j < small.size(); j++) {
			cout << tableMap[both[i]][small[j]] << '\t';
		}
		cout << both[i] << endl;
	}
	cout << endl;

//Elemzés
	string input = "aabbdcc#";
	string allapot = "S#";

	while (true) {

		cout << "Input: " << input << "\tÁllapot: " << allapot << endl;

		int number = tableMap[allapot[0]][input[0]];

		if (number > 0) {
			allapot = rules[number - 1].second + allapot.substr(1);	
		} else if (number == -1) {
			input = input.substr(1);
			allapot = allapot.substr(1);
		} else if (number == -2) {
			cout << "A szó eleme a nyelvnek..." << endl;
			break;
		} else {
			cout << "Error" << endl;
			break;
		}

	}

	return 0;
}

