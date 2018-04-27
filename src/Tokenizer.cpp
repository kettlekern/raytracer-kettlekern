#include "Tokenizer.h"

using namespace std;

vector<string> Tokenizer::split(string str, string delim) {
	string temp2, temp = str;
	int index = str.find(delim);
	vector<string> retval;
	if (index < 0) {
		retval.push_back(str);
	}
	else {
		while (temp.length() > temp.find(delim)) {
			index = temp.find(delim);
			temp2 = temp;
			temp = temp.substr(0, temp.find(delim));
			if (temp != "") {
				retval.push_back(temp);
			}
			temp = temp2.substr(index + delim.length(), temp2.length() - 1);
			if (temp != "" && !(temp.length() > temp.find(delim))) {
				retval.push_back(temp);
			}
		}
	}
	return retval;
}

void Tokenizer::addSplit(const std::string & delim, vector<std::string> &vec)
{
	vector<string> temp1, temp2;
	for (string str : vec) {
		temp1 = split(str, delim);
		temp2.insert(temp2.end(), temp1.begin(), temp1.end());
	}
	vec = temp2;
}

void Tokenizer::readInput() {
	string tok;
	while (tokens.empty()) {
		//Read the input from the stream
		(*input) >> tok;
		//clean the input of unwanted characters
		vector<string> vec = split(tok, ",");
		//After the first read, the next reads get more complex, so this method is used for subsequent characters
		addSplit("<", vec);
		addSplit(">", vec);

		//Put the input into the queue
		for (const auto & str : vec) {
			tokens.push(str);
		}
	}
}

std::string Tokenizer::getToken() {
	string tok;
	if (tokens.size() == 0) {
		readInput();
	}
	tok = tokens.front();
	if (!tokens.empty()) {
		tokens.pop();
	}

	if (tok.substr(0, 1) == "{") {
		if (tok.length() > 1) {
			tokens.push(tok.substr(1, tok.length()));
			tok = "{";
		}
	}
	else if (tok.length() > 0 && tok.substr(tok.length() - 1, tok.length()) == "}") {
		if (tok.length() > 1) {
			tokens.push(tok.substr(tok.length() - 1, tok.length()));
			tok = tok.substr(0, tok.length() - 1);
		}
	}

	return tok;
}

void Tokenizer::getLine(string & val) {
	if (!tokens.empty()) {
		tokens.pop();
	}
	getline((*input), val);
}