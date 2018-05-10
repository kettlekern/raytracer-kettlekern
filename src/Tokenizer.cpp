#include "Tokenizer.h"

using namespace std;

vector<string> Tokenizer::split(string str, string delim, bool add) {
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
			if (add) {
				retval.push_back(delim);
			}
			temp = temp2.substr(index + delim.length(), temp2.length() - 1);
			if (temp != "" && !(temp.length() > temp.find(delim))) {
				retval.push_back(temp);
			}
		}
	}
	return retval;
}

void Tokenizer::addSplit(const std::string & delim, vector<std::string> &vec, bool add)
{
	vector<string> temp1, temp2;
	for (string str : vec) {
		temp1 = split(str, delim, add);
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
		vector<string> vec = split(tok, ",", false);
		//After the first read, the next reads get more complex, so this method is used for subsequent characters
		addSplit("<", vec, false);
		addSplit(">", vec, false);
		addSplit("{", vec, true);
		addSplit("}", vec, true);

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

	return tok;
}

void Tokenizer::getLine(string & val) {
	if (!tokens.empty()) {
		tokens.pop();
	}
	getline((*input), val);
}