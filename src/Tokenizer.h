#pragma once
#ifndef __TOKENIZER_H_473_
#define __TOKENIZER_H_473_

#include <string>
#include <queue>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

class Tokenizer {
	std::istringstream* input;
	std::queue<std::string> tokens;
	std::vector<std::string> split(std::string str, std::string delim, bool add);
	void addSplit(const std::string & delim, std::vector<std::string> &vec, bool add);
	void readInput();
public:
	Tokenizer(std::istringstream* input) { this->input = input; }
	std::string getToken();
	void getLine(std::string & val);
};



#endif