#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <sstream>
#include <cmath>
#include <stdexcept>
#include <ctime>
#include <algorithm>

class PmergeMe {
private:
	PmergeMe(const PmergeMe& other);
	PmergeMe& operator=(const PmergeMe& other);
	std::vector<int> _argv;
	
public:
	PmergeMe();
	~PmergeMe();
	void checkArg(const std::string& arg);
	void argvPrint();
	
	class VectorSort {
	private:
		VectorSort(const VectorSort& other);
		VectorSort& operator=(const VectorSort& other);
		std::vector<int> _argv;
		std::vector< std::vector<int> > _main;
		std::vector< std::vector<int> > _pending;
		std::vector<size_t> _jacobNumbers;
		std::vector<size_t> _insertOrder;
	public:
		VectorSort();
		~VectorSort();
		void beforePrint();
		void afterPrint();
		size_t getMainSize();
		void checkArg(const std::string& arg);
		int binarySearchWithPair(int high);
		size_t getJacobsthalNumber(size_t n);
		void makeJacobsthalNumbers();
		void setInsertOrder();
		bool compareFunction(const std::vector<int>::iterator &a, const std::vector<int>::iterator &b);
		void insertBInA(size_t unitSize);
		size_t binarySearch(int insertNum);
		void recur(size_t size);
		void doSort();
	};
};

#endif