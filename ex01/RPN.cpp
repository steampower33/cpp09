#include "RPN.hpp"

RPN::RPN() {}

RPN::RPN(const RPN& other) {
	*this = other;
}

RPN& RPN::operator=(const RPN& other) {
	if (this != &other)
	{
		_stack = other._stack;
	}
	return *this;
}

RPN::~RPN() {}

int RPN::isTokenNum(std::string& token) {
	std::string tokens = "+-/*";

	if (token.length() == 1 && (tokens.find(token) != std::string::npos))
		return 0;
	if (token.length() == 1 && std::isdigit(token[0]))
		return 1;
	else
		throw std::runtime_error("Error");
}

void RPN::polish(const std::string& arg) {
	std::istringstream iss(arg);

	std::string token;
	char *endptr = NULL;
	double tokenValue = 0;
	double num1, num2, calcNum;
	std::size_t idx;
	std::string tokens = "+-/*";
	while (iss >> token) {
		if (isTokenNum(token))
		{
			endptr = NULL;
			tokenValue = strtod(token.c_str(), &endptr);
			_stack.push(tokenValue);
		} else {
			num2 = _stack.top();
			_stack.pop();
			num1 = _stack.top();
			_stack.pop();
			idx = tokens.find(token);
			switch (idx) {
				case 0:
					calcNum = num1 + num2;
					break ;
				case 1:
					calcNum = num1 - num2;
					break ;
				case 2:
					calcNum = num1 / num2;
					break ;
				case 3:
					calcNum = num1 * num2;
					break ;
				default:
					throw std::runtime_error("Error");
			}
			_stack.push(calcNum);
		}
	}
	std::cout << _stack.top() << std::endl;
	_stack.pop();
}