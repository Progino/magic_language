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
		string operation;
		int value;
		int secondValue;
		ss >> varName >> equalSign >> value >> operation >> secondValue;

		if (equalSign == "=") {
			if (operation == "+") {
				memory[varName] = value + secondValue;
			}
			else if (operation == "-") {
				memory[varName] = value - secondValue;
			}
			else if (operation == "") {
				memory[varName] = value;
			}
			else {
				cout << "Spell failed: unknown operation" << endl;
				return;

			}
		}
		else {
			cout << "Spell failed: missing '=' sign" << endl;
			return;
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
	
	vector <string>  programCode;

	cout << "Write your code below" << endl;
	cout << "Type 'run' to execute everything,or 'vanish' to quit" << endl;

	while (true) {
		cout << "["<<programCode.size() +1 << "] ";
		getline(cin, line);

		if (line == "vanish") {
			break;
		}
		if (line == "run") {
			cout << "\n--- [Executing Spellbook...] ---" << endl;

			for (const string& storedLine : programCode) {
				interpretLine(storedLine);
			}

			cout << "--- [Spellbook Execution Complete] ---\n" << endl;
			programCode.clear();
		}
		else {
			programCode.push_back(line);
		}
	}

	cout << "The portal is closing... Vanished!" <<endl;
	return 0;
}