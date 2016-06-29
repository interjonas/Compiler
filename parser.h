#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>
#pragma once

class symbolTable {
public:
	int value;
	std::string symbol;
	bool marked;
	symbolTable()
	{
		marked = false;
	}
};
class var_decl{
public:
	int count;
	int variable;
	std::string list[20];
	var_decl()
	{
		count = 0;
	}
};
