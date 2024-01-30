#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>
#include <stdexcept>
#include <utility>

class PmergeMe {
private:
	std::vector<int> _argv;
	std::vector< std::pair<int, int> > _pair;
	std::vector<int> _main;
	std::vector<int> _pending;
	PmergeMe(const PmergeMe& other);
	PmergeMe& operator=(const PmergeMe& other);
public:
	PmergeMe();
	~PmergeMe();
	void checkArg(const std::string& arg);
	void makePair();
	void mergeSort(int low, int high);
	void merge(int low, int mid, int high);
	void swapPair(std::pair<int, int>& left, std::pair<int, int>& right);
	void pairSort();
	void splitMainPending();
};

#endif