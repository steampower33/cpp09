#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>

class PmergeMe {
private:
	std::vector<int> _vector;
	PmergeMe(const PmergeMe& other);
	PmergeMe& operator=(const PmergeMe& other);
public:
	PmergeMe();
	~PmergeMe();
	void checkArg(const std::string& arg);
	void merge();
};

#endif