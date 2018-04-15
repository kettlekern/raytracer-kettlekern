#pragma once
#ifndef __TOKENIZER_H_473_
#define __TOKENIZER_H_473_

#include <string>
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>

class Tokenizer {
	std::istringstream* input;
	std::queue<std::string> tokens;
public:
	Tokenizer(std::istringstream* input) { this->input = input; }
	std::string getToken();
	void getLine(std::string & val);
};



#endif