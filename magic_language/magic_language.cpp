#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;

const string FILENAME = "magic_language.txt";

unordered_map<string, int> memory;

void loadProgram(vector <string> &program) {
	ifstream file(FILENAME);

	if (!file.is_open()) {
		cout << "failed to open the file" << endl;
		return;
	}

	string line;
	while (getline(file, line)) {
		program.push_back(line);
	}

	file.close();
	return;
}

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

	while (!command.empty() && (isspace((unsigned char)command.back()) || command.back() == '\r' || command.back() == '\n')) {
		command.pop_back();
	}
	return command;
}

int findEnd(vector <string> program, int &startLine) {
	int lineCheck = 1;
	string command;
	string line;
	string code;

	while (startLine < program.size()) {
		line = program[startLine];
		code = comment(line);
		stringstream ss(code);
		ss >> command;
		if (command == "prophecy" || command == "echo" || command == "ritual") {
			lineCheck++;
		}
		else if(command == "end"){
			lineCheck--;
			if (lineCheck == 0) {
				return startLine;
			}
		}
		startLine++;
	}
	cout << "Spell failed: missing 'end' keyword" << endl;
	return lineCheck;
}

void interpretLine(vector <string> program, int &lineNumber) {
	string line;
	line = program[lineNumber];
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
		bool first = true;
		while (getline(ss >> ws, target, '!')) {

			while (!target.empty() && isspace((unsigned char)target.front())) {
				target.erase(target.begin());
			}
			while (!target.empty() && isspace((unsigned char)target.back())) {
				target.pop_back();
			}
			if (target.size() >= 2 && target.front() == '"' && target.back() == '"') {
				target.erase(target.begin());
				target.pop_back();
			}

			if (!target.empty()) {

				if (!first) {
					cout << " ";
				}
				first = false;

				if (memory.find(target) != memory.end()) {
					cout << memory[target];
				}
				else {
					cout << target;
				}
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
		//prophecy x > y (if)
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

		int finalLine;
		int searchLine = lineNumber + 1;
		finalLine = findEnd(program,searchLine);
		if(finalLine == -1) return;

		if (correct) {
			int innerLine = lineNumber + 1;
			while (innerLine < finalLine) {
				interpretLine(program, innerLine);
				innerLine++;
			}
		}
		lineNumber = finalLine;
	}

	else if (command == "echo") {
		//echo 5 (for)
		string cycles;
		ss >> cycles;

		int finalLine;
		int searchLine = lineNumber + 1;
		finalLine = findEnd(program, searchLine);
		if (finalLine == -1) return;
		int startLine = lineNumber + 1;

		for (int i = 0;i < getValue(cycles); i++) {
			int innerLine = startLine;
			while (innerLine < finalLine) {
				interpretLine(program, innerLine);
				innerLine++;
			}

		}
		lineNumber = finalLine;
	}

	else if (command == "ritual") {
		//ritual x > y (while)
		string firstValue;
		string secondValue;
		string operation;
		ss >> firstValue >> operation >> secondValue;

		int finalLine;
		int searchLine = lineNumber + 1;
		finalLine = findEnd(program, searchLine);
		if (finalLine == -1) return;
		int startLine = lineNumber + 1;

		while (true) {
			bool valid = false;
			bool correct = false;
			int valueOne = getValue(firstValue);
			int valueTwo = getValue(secondValue);

			if (operation == "bound") {
				valid = true;
				if (valueOne == valueTwo) {
					correct = true;
				}
			}
			else if (operation == "unbound") {
				valid = true;
				if (valueOne != valueTwo) {
					correct = true;
				}
			}
			else if (operation == "weaker") {
				valid = true;
				if (valueOne < valueTwo) {
					correct = true;
				}
			}
			else if (operation == "weakerAs") {
				valid = true;
				if (valueOne <= valueTwo){
					correct = true;
				}
			}
			else if (operation == "stronger") {
				valid = true;
				if (valueOne > valueTwo) {
					correct = true;
				}
			}
			else if (operation == "strongerAs") {
				valid = true;
				if (valueOne >= valueTwo) {
					correct = true;
				}
			}
			if (!valid) {
				cout << "Spell failed: invalid operator" << endl;
				return;
			}

			if (!correct) {
				break;
			}

			int innerLine = startLine;
			while (innerLine < finalLine) {
				interpretLine(program, innerLine);
				innerLine++;
			}
		}
		lineNumber = finalLine;
	}
}

int main() {
	string line;
	vector <string>  programCode;
	int lineNumber = 0;
	loadProgram(programCode);


	cout << "summoning the code" << endl<<endl;

	while (lineNumber < programCode.size()){
		interpretLine(programCode, lineNumber);
		lineNumber++;
	}

	cout << endl<<"The portal is closing... Vanished!"<<endl;
	return 0;
}