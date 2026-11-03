#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
using namespace std;

unordered_map<string, int> memory;

void interpretLine(const string& line) {
	stringstream ss(line);
	string command;
	ss >> command;

	if (command.empty()) {
		return;
	}

	if (command == "reveal") {
		string target;
		getline(ss >> ws, target);

		if (memory.find(target) != memory.end()) {
			cout << memory[target] << endl;
		}
		else {
			cout << target << endl;
		}
	}

	else if (command == "conjure") {
		string varName;
		string equalSign;
		int value;
		ss >> varName >> equalSign >> value;

		if (equalSign == "=") {
			memory[varName] = value;
			cout << "Spell cast: " << varName << " is now " << value << endl;
		}
		else {
			cout << "Spell failed: missing '=' sign" << endl;
		}
	}

	else if (command == "infuse") {
		string target;
		ss >> target;
		if (memory.find(target) != memory.end()) {
			memory[target]++;
		}
		else {
			cout << "Spell failed: variable not found" << endl;
		}
	}

	else if (command == "drain") {
		string target;
		ss >> target;
		if (memory.find(target) != memory.end()) {
			memory[target]--;
		}
		else {
			cout << "Spell failed: variable not found" << endl;
		}
	}
}

int main() {
	string line;
	cout << "type your spells below, type vanish to close the program" << endl;

	while (true) {
		cout << '>';
		getline(cin, line);

		if (line == "vanish") {
			break;
		}
		interpretLine(line);
	}

	cout << "Exiting the program"<<endl;
	return 0;
}