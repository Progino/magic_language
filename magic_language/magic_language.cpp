#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <fstream>
using namespace std;



unordered_map<string, int> memory;

int getValue(const string& token) {
	if (isdigit(token[0]) || (token.size() > 1 && token[0] == '-')) {
		return stoi(token);
	}
	if (memory.find(token) != memory.end()) {
		return memory[token];
	}

	cout << "Warning: " << token << " is undefined. Defaulting to 0." << endl;
	return 0;
}

void interpretLine(const string& line) {
	stringstream ss(line);
	string command;
	ss >> command;

	if (command.empty()) {
		return;
	}

	if (command == "reveal") {
		string target;
		getline(ss >> ws, target, '!');
		if (target.empty()) {
			cout << endl;
		}
		if (memory.find(target) != memory.end()) {
			cout << memory[target];
		}
		else {
			cout << target;
		}


		while (getline(ss, target, '!')) {

			if (target.empty()) {
				cout << endl;
			}
			if (memory.find(target) != memory.end()) {
				cout << memory[target];
			}
			else {
				cout << target;
			}
		}
		cout << endl;
	}

	if (command == "invoke") {
		string target;
		int value;
		getline(ss >> ws, target);
		if (target.empty()) {
			cout << "Spell failed: no message provided" << endl;
			return;
		}
		if (memory.find(target) != memory.end()) {
			cin >> value;
			memory[target] = value;
		}
		else {
			cout << "Spell failed: variable not found" << endl;
		}
	}

	else if (command == "summon") {
		string varName;
		string equalSign;
		string operation;
		string firstToken;
		string secondToken;
		ss >> varName >> equalSign >> firstToken;

		if (equalSign == "=") {
			int firstValue = getValue(firstToken);

				if (ss >> operation >> secondToken){

					int secondValue = getValue(secondToken);

					if (operation == "+") {
						memory[varName] = firstValue + secondValue;
					}
					else if (operation == "-") {
						memory[varName] = firstValue - secondValue;
					}
					else if (operation == "*") {
						memory[varName] = firstValue * secondValue;
					}
					else if (operation == "/") {
						memory[varName] = firstValue / secondValue;
					}
					else {
						cout << "Spell failed: unknown operation" << endl;
						return;

					}
				}
				else {
					memory[varName] = firstValue;
				}
		}
		else{
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

	else if (command == "prophecy") {
		string firstVariable;
		string secondVariable;
		string operation;
		bool valid = false;
		ss >> firstVariable >> operation >> secondVariable;
		if (operation == "bound") {
			if (firstVariable == secondVariable) {
				valid = true;
			}
		}
		else if (operation == "weaker") {
			if (firstVariable < secondVariable) {
				valid = true;
			}
		}
		else if (operation == "stronger") {
			if (firstVariable > secondVariable) {
				valid = true;
			}
		}
		string fulfill;
		ss >> fulfill;
		if (fulfill == "fulfill") {
			if (valid) {
				string function;
				getline(ss >> ws, function);
				interpretLine(function);
			}
		}
		else {
			cout << "Spell failed: missing 'fulfill' keyword" << endl;
		}

	}
}

int main() {
	string line;
	
	vector <string>  programCode;

	cout << "Write your code below" << endl;
	cout << "Type 'run' to execute everything,or 'vanish' to quit" << endl<<endl;

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

	cout << endl<<"The portal is closing... Vanished!"<<endl;
	return 0;
}