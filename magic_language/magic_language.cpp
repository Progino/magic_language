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

string comment(string line) {
	stringstream ss(line);
	string command;
	getline(ss >> ws, command, '^');

	while (!command.empty() && isspace(command.back())) {
		command.pop_back();
	}
	return command;
}

void interpretLine(const string& line) {
	string code;
	code = comment(line);
	stringstream ss(code);
	string command;
	string comment;
	ss >> command;

	if (command.empty()) {
		return;
	}

	if (command == "reveal") {
		string target;
		getline(ss >> ws, target, '!');

		while (!target.empty() && (isspace(target.front()) || target.front() == '"')) {
			if (target.front() == '"') {
				target.erase(target.begin());
				break;
			}
			target.erase(target.begin());
		}

		while (!target.empty() && (isspace(target.back()) || target.back() == '"')) {
			if (target.back() == '"') {
				target.pop_back();
				break;
			}
			target.pop_back();
		}
		if (target.empty()) {
			cout << endl;
		}
		if (memory.find(target) != memory.end()) {
			cout << memory[target];
		}
		else {
			cout << target;
		}


		while (getline(ss >> ws, target, '!')) {

			while (!target.empty() && (isspace(target.front()) || target.front() == '"')) {
				if (target.front() == '"') {
					target.erase(target.begin());
					break;
				}
				target.erase(target.begin());
			}

			while (!target.empty() && (isspace(target.back()) || target.back() == '"')) {
				if (target.back() == '"') {
					target.pop_back();
					break;
				}
				target.pop_back();
			}
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

	else if (command == "invoke") {
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
		//prophecy x > y fulfill drain x
		string firstVariable;
		string secondVariable;
		string operation;
		bool valid = false;
		bool correct = false;
		ss >> firstVariable >> operation >> secondVariable;
		if (operation == "bound") {
			valid = true;
			if (getValue(firstVariable) == getValue(secondVariable)) {
				correct = true;
			}
		}
		else if (operation == "unbound") {
			valid = true;
			if (getValue(firstVariable) != getValue(secondVariable)) {
				correct =  true;
			}
		}
		else if (operation == "weaker") {
			valid = true;
			if (getValue(firstVariable) < getValue(secondVariable)) {
				correct = true;
			}
		}
		else if (operation == "weakerAs") {
			valid = true;
			if (getValue(firstVariable) <= getValue(secondVariable)) {
				correct = true;
			}
		}
		else if (operation == "stronger") {
			valid = true;
			if (getValue(firstVariable) > getValue(secondVariable)) {
				correct = true;
			}
		}
		else if (operation == "strongerAs") {
			valid = true;
			if (getValue(firstVariable) >= getValue(secondVariable)) {
				correct = true;
			}
		}

		if (!valid) {
			cout << "Spell failed: invalid operator" << endl;
			return;
		}

		string fulfill;
		ss >> fulfill;
		if (fulfill == "fulfill") {
			if (correct) {
				string function;
				getline(ss >> ws, function);
				interpretLine(function);
			}
		}
		else {
			cout << "Spell failed: missing 'fulfill' keyword" << endl;
		}
	}

	else if (command == "echo") {
		//echo 5 fulfill drain x
		string function;
		string cycles;
		string fulfill;
		ss >> cycles >> fulfill;
		if (fulfill == "fulfill") {
			getline(ss >> ws, function);
			for (int i = 0;i < getValue(cycles); i++) {
				interpretLine(function);
			}
		}
		else {
			cout << "Spell failed: missing 'fulfill' keyword" << endl;
			return;
		}
	}

	else if (command == "ritual") {
		//ritual x > y fulfill drain x
		string firstValue;
		string secondValue;
		string operation;
		string fulfill;
		ss >> firstValue >> operation >> secondValue >> fulfill;

		if (fulfill != "fulfill") {
			cout << "Spell failed: missing 'fulfill' keyword";
			return;
		}
		string function;
		getline(ss >> ws, function);

		while (true) {
			bool valid = false;
			int valueOne = getValue(firstValue);
			int valueTwo = getValue(secondValue);
			if (operation == "bound") {
				valid = true;
				if (valueOne == valueTwo) {
					interpretLine(function);
					continue;
				}
				else {
					return;
				}
			}
			else if (operation == "unbound") {
				valid = true;
				if (valueOne != valueTwo) {
					interpretLine(function);
					continue;
				}
				else {
					return;
				}
			}
			else if (operation == "weaker") {
				valid = true;
				if (valueOne < valueTwo) {
					interpretLine(function);
					continue;
				}
				else {
					return;
				}
			}
			else if (operation == "weakerAs") {
				valid = true;
				if (valueOne <= valueTwo){
					interpretLine(function);
					continue;
				}
				else {
					return;
				}
			}
			else if (operation == "stronger") {
				valid = true;
				if (valueOne > valueTwo) {
					interpretLine(function);
					continue;
				}
				else {
					return;
				}
			}
			else if (operation == "strongerAs") {
				valid = true;
				if (valueOne >= valueTwo) {
					interpretLine(function);
					continue;
				}
				else {
					return;
				}
			}
			if (!valid) {
				cout << "Spell failed: invalid operator" << endl;
				return;
			}
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