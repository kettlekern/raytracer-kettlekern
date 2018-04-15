#include "Tokenizer.h"

using namespace std;

std::string Tokenizer::getToken() {
	string tok;
	if (tokens.size() > 0) {
		tok = tokens.front();
		if (!tokens.empty()) {
			tokens.pop();
		}
	}
	else {
		(*input) >> tok;
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