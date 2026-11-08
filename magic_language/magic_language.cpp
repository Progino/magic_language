#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <vector>
#include <random>
using namespace std;

const string FILENAME = "magic_language.txt";

unordered_map<string, int> memory;
unordered_map<string, vector<int>> grimmoires;
bool spellFailed = false;

void loadProgram(vector <string> &program) {
	ifstream file(FILENAME);

	if (!file.is_open()) {
		cout << "Spell failed: failed to open the file" << endl;
		return;
	}

	string line;
	while (getline(file, line)) {
		program.push_back(line);
	}

	file.close();
	return;
}

int getValue(const string& token, int lineNumber) {
	if (token.empty()) {
		return 0;
	}
	if (isdigit(token[0]) || (token.size() > 1 && token[0] == '-')) {
		return stoi(token);
	}
	if (memory.find(token) != memory.end()) {
		return memory[token];
	}

	cout << "Warning (Line " << (lineNumber + 1) << "): "<< token << " is undefined. Defaulting to 0." << endl;
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

int findEnd(vector <string> &program, int &startLine) {
	int lineCheck = 1;
	string command;
	string line;
	string code;

	while (startLine < program.size()) {
		line = program[startLine];
		code = comment(line);
		stringstream ss(code);
		ss >> command;
		if (command == "prophecy" || command == "echo" || command == "ritual" || command == "ward") {
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

void findCurse(vector <string> &program, int startLine, int& curseLine, int& endLine) {
	curseLine = -1;
	endLine = -1;

	while (startLine < program.size()) {
		stringstream ss(comment(program[startLine]));
		string command;
		ss >> command;

		if (command == "prophecy" || command == "echo" || command == "ritual" || command == "ward") {
			int nextLine = startLine + 1;
			startLine = findEnd(program, nextLine);
			if (startLine == -1) return;
		}

		else if (command == "curse") {
			curseLine = startLine;
		}
		else if (command == "end") {
			int finalSearch = startLine;
			endLine = findEnd(program, finalSearch);
			return;
		}
		startLine++;
	}
	cout << "Spell failed: missing 'end' keyword" << endl;
}

void interpretLine(vector <string> &program, int &lineNumber) {
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
		//invoke x(cin)
		string target;
		int value;
		getline(ss >> ws, target);
		if (target.empty()) {
			cout << "Spell failed (Line "<<(lineNumber + 1)<< "): no message provided" << endl;
			spellFailed = true;
			return;
		}
		if (memory.find(target) != memory.end()) {
			cin >> value;
			memory[target] = value;
		}
		else {
			cout << "Spell failed (Line " << (lineNumber + 1) << "): variable not found" << endl;
			spellFailed = true;
			return;
		}
	}

	else if (command == "summon") {
		//summon x = 10
		string varName;
		string equalSign;
		string operation;
		string firstToken;
		string secondToken;
		ss >> varName >> equalSign >> firstToken;

		if (equalSign == "=") {
			if (firstToken == "") {
				cout << "Spell failed (Line " << (lineNumber + 1) << "): incomplete command" << endl;
				spellFailed = true;
				return;
			}

			if (firstToken == "chaos") {
				string minimumS;
				string maximumS;
				ss >> minimumS >> maximumS;

				if (minimumS == "" || maximumS == "") {
					cout << "Spell failed (Line " << (lineNumber + 1) << "): chaos requires two limits" << endl;
					spellFailed = true;
					return;
				}

				int minimum = getValue(minimumS,lineNumber);
				int maximum = getValue(maximumS,lineNumber);

				if (maximum < minimum) {
					cout << "Spell failed (Line " << (lineNumber + 1) << "): chaos limits are inverted" << endl;
					spellFailed = true;
					return;
				}

				memory[varName] = rand() % (maximum - minimum + 1) + minimum;
				return;
			}

			int firstValue = getValue(firstToken,lineNumber);

			if (ss >> operation >> secondToken){

				int secondValue = getValue(secondToken,lineNumber);

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
					if (secondValue == 0) {
						cout << "Spell failed (Line " << (lineNumber + 1) << "): division by zero" << endl;
						spellFailed = true;
						return;
					}
					memory[varName] = firstValue / secondValue;
					return;
				}
				else {
					cout << "Spell failed (Line " << (lineNumber + 1) << "): unknown operation" << endl;
					spellFailed = true;
					return;

				}
			}
			else {
				memory[varName] = firstValue;
			}
		}
		else{
			cout << "Spell failed (Line " << (lineNumber + 1) << "): missing '=' sign" << endl;
			spellFailed = true;
			return;
		}
	}

	else if (command == "banish") {
		// banish x
		string var;
		bool banished = false;
		ss >> var;

		if (memory.find(var) != memory.end()) {
			memory.erase(var);
			banished = true;
		}
		if (grimmoires.find(var) != grimmoires.end()) {
			grimmoires.erase(var);
			banished = true;
		}
		if(!banished) {
			cout << "Spell failed (Line " << (lineNumber + 1) << "): cannot banish the nonexistent" << endl;
			spellFailed = true;
			return;
		}	
	}

	else if (command == "grimmoire") {
		//grimmoire x 10(vector)
		string vectorName;
		string sizeS;
		int size;
		ss >> vectorName >> sizeS;
		size = getValue(sizeS,lineNumber);
		if (size <= 0) {
			cout << "Spell failed (Line " << (lineNumber + 1) << "): invalid size" << endl;
			spellFailed = true;
			return;
		}
		grimmoires[vectorName] = vector<int>(size,0);

	}

	else if (command == "scribe") {
		//scribe x 0 y
		string vectorName;
		string positionS;
		string varString;
		ss >> vectorName >> positionS >> varString;
		int position = getValue(positionS, lineNumber);
		int var = getValue(varString,lineNumber);

		if (grimmoires.find(vectorName) != grimmoires.end()) {
			if (grimmoires[vectorName].size() > position && position >= 0) {
				grimmoires[vectorName][position] = var;
			}
			else {
				cout << "Spell failed (Line " << (lineNumber + 1) << "): invalid position" << endl;
				spellFailed = true;
				return;
			}
		}
		else {
			cout << "Spell failed (Line " << (lineNumber + 1) << "): vector not found" << endl;
			spellFailed = true;
			return;
		}
	}

	else if (command == "extract") {
		//extract x 0 y
		string vectorName;
		string positionS;
		string varString;
		ss >> vectorName >> positionS >> varString;
		int position = getValue(positionS,lineNumber);
		if (grimmoires.find(vectorName) != grimmoires.end()) {
			if (grimmoires[vectorName].size() > position && position >= 0) {
				if (memory.find(varString) != memory.end()) {
					memory[varString] = grimmoires[vectorName][position];
				}
				else {
					cout << "Spell failed (Line " << (lineNumber + 1) << "): variable not found" << endl;
					spellFailed = true;
					return;
				}
			}
			else {
				cout << "Spell failed (Line " << (lineNumber + 1) << "): invalid position" << endl;
				spellFailed = true;
				return;
			}
		}
		else {
			cout << "Spell failed (Line " << (lineNumber + 1) << "): vector not found" << endl;
			spellFailed = true;
			return;
		}
	}

	else if (command == "infuse") {
		//infuse x(++)
		string target;
		ss >> target;
		if (memory.find(target) != memory.end()) {
			memory[target]++;
		}
		else {
			cout << "Spell failed (Line " << (lineNumber + 1) << "): variable not found" << endl;
			spellFailed = true;
			return;
		}
	}

	else if (command == "drain") {
		//drain x(--)
		string target;
		ss >> target;
		if (memory.find(target) != memory.end()) {
			memory[target]--;
		}
		else {
			cout << "Spell failed (Line " << (lineNumber + 1) << "): variable not found" << endl;
			spellFailed = true;
			return;
		}
	}

	else if (command == "prophecy") {
		//prophecy x > y (if)
		string firstVariable;
		string secondVariable;
		string operation;
		bool found = false;
		bool valid = false;
		bool correct = false;
		ss >> firstVariable >> operation >> secondVariable;
		if (memory.find(firstVariable) != memory.end()) {
			found = true;
		}
		if (memory.find(secondVariable) != memory.end()) {
			found = true;
		}
		if (!found) {
			cout << "Spell Failed (Line " << (lineNumber + 1) << "): variable not found" << endl;
			spellFailed = true;
			return;
		}

		if (operation == "bound") {
			valid = true;
			if (getValue(firstVariable,lineNumber) == getValue(secondVariable,lineNumber)) {
				correct = true;
			}
		}
		else if (operation == "unbound") {
			valid = true;
			if (getValue(firstVariable,lineNumber) != getValue(secondVariable,lineNumber)) {
				correct =  true;
			}
		}
		else if (operation == "weaker") {
			valid = true;
			if (getValue(firstVariable,lineNumber) < getValue(secondVariable,lineNumber)) {
				correct = true;
			}
		}
		else if (operation == "weakerAs") {
			valid = true;
			if (getValue(firstVariable, lineNumber) <= getValue(secondVariable,lineNumber)) {
				correct = true;
			}
		}
		else if (operation == "stronger") {
			valid = true;
			if (getValue(firstVariable, lineNumber) > getValue(secondVariable,lineNumber)) {
				correct = true;
			}
		}
		else if (operation == "strongerAs") {
			valid = true;
			if (getValue(firstVariable,lineNumber) >= getValue(secondVariable,lineNumber)) {
				correct = true;
			}
		}

		if (!valid) {
			cout << "Spell failed (Line " << (lineNumber + 1) << "): invalid operator" << endl;
			spellFailed = true;
			return;
		}

		int finalLine;
		int searchLine = lineNumber + 1;
		finalLine = findEnd(program,searchLine);
		if (finalLine == -1) {
			cout << "Spell failed (Line " << (lineNumber + 1) << "): prophecy block is never closed with 'end'" << endl;
			spellFailed = true;
			return;
		}

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

		for (int i = 0;i < getValue(cycles,lineNumber); i++) {
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
			int valueOne = getValue(firstValue,lineNumber);
			int valueTwo = getValue(secondValue,lineNumber);

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
				cout << "Spell failed (Line " << (lineNumber + 1) << "): invalid operator" << endl;
				spellFailed = true;
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

	else if (command == "ward") {
		int curseLine;
		int endLine;
		findCurse(program, lineNumber + 1, curseLine, endLine);
		if (endLine == -1) {
			return;
		}
		spellFailed = false;

		int protectedEnd;
		if (curseLine != -1) {
			protectedEnd = curseLine;
		}
		else {
			protectedEnd = endLine;
		}

		int innerLine = lineNumber + 1;
		while (innerLine < protectedEnd) {
			interpretLine(program, innerLine);
			if (spellFailed) {
				break;
			}
			innerLine++;
		}
		if (spellFailed && curseLine != -1) {
			spellFailed = false;
			int backupLine = curseLine + 1;
			while (backupLine < endLine) {
				interpretLine(program, backupLine);
				backupLine++;
			}
		}
		lineNumber = endLine;
	}

	else if (command == "alchemy") {
		bool error = false;
		string firstVariable;
		string secondVariable;
		ss >> firstVariable >> secondVariable;

		if (memory.find(firstVariable) == memory.end()) {
			error = true;
		}
		if (memory.find(secondVariable) == memory.end()) {
			error = true;
		}
		if (firstVariable == "" || secondVariable == "") {
			error = true;
		}
		if (error) {
			cout << "Spell failed (line " << lineNumber + 1 << " ): variable not found" << endl;
			spellFailed = true;
			return;
		}

		int firstVar = getValue(firstVariable, lineNumber);
		int secondVar = getValue(secondVariable, lineNumber);
		
		memory[firstVariable] = secondVar;
		memory[secondVariable] = firstVar;
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
		if (spellFailed) {
			cout << "[CRITICAL ERROR] The ritual was violently broken!" << endl;
			break;
		}
		lineNumber++;
	}

	cout << endl<<"The portal is closing... Vanished!"<<endl;
	return 0;
}