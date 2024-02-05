#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <stack>
#include <sstream>
#include <cmath>
#include <stdexcept>
#include <climits>

class RPN {
private:
	std::stack<int> _stack;
	RPN(const RPN& other);
	RPN& operator=(const RPN& other);
public:
	RPN();
	~RPN();
	void polish(const std::string& arg);
	int isTokenNum(std::string& token);
};

#endif