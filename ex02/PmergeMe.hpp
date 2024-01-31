#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>
#include <stdexcept>
#include <ctime>

class PmergeMe {
private:
	std::vector<int> _argv;
	std::vector< std::pair<int, int> > _pair;
	std::vector<int> _main;
	std::vector<int> _pending;
	std::vector<size_t> _jacobNumbers;
	std::vector<size_t> _insertOrder;
	PmergeMe(const PmergeMe& other);
	PmergeMe& operator=(const PmergeMe& other);
public:
	PmergeMe();
	~PmergeMe();
	void checkArg(const std::string& arg);
	void makePair();
	void mergeSort(size_t low, size_t high);
	void merge(size_t low, size_t mid, size_t high);
	void swapPair(std::pair<int, int>& left, std::pair<int, int>& right);
	void doSort();
	void splitMainPending();
	size_t getJacobsthalNumber(size_t n);
	void makeJacobsthalNumbers();
	void setInsertOrder();
	void insertionSort();
	size_t binarySearch(int insertNum);
	void beforePrint();
	void afterPrint();
};

#endif