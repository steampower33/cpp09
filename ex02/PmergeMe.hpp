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
	void checkArg(char** argv);
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
		void doSort(char** argv);
		void checkArg(char** argv);
		void recur(size_t size);
		void swapMainPending(size_t size, size_t loopSize);
		void splitMainPending(size_t size, size_t loopSize);
		void makeJacobsthalNumbers();
		size_t getJacobsthalNumber(size_t n);
		void setInsertOrder();
		void insertBInA(size_t unitSize);
		size_t binarySearch(int insertNum, int high);
		void moveMainToArgv(size_t size);
		void clearVector();
		void checkSort();
		void afterPrint();
		size_t getArgvSize();
	};
};

#endif