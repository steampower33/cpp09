#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <sstream>
#include <cmath>
#include <stdexcept>
#include <ctime>

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
	
	class VecterSort {
	private:
		VecterSort(const VecterSort& other);
		VecterSort& operator=(const VecterSort& other);
		std::vector<int> _argv;
		std::vector<int> _main;
		std::vector<int> _pending;
		std::vector<size_t> _jacobNumbers;
		std::vector<size_t> _insertOrder;
	public:
		VecterSort();
		~VecterSort();
		size_t getMainSize();
		void checkArg(const std::string& arg);
		void changePairPos(int low, int high);
		int binarySearchWithPair(int high);
		void eraseAndInsert(int high, int insertPos);
		void makePair(int idx);
		void doSort();
		void splitMainPending();
		size_t getJacobsthalNumber(size_t n);
		void makeJacobsthalNumbers();
		void setInsertOrder();
		void insertionSort();
		size_t binarySearch(int insertNum);
		void beforePrint();
		void afterPrint();
		void checkSort();
	};
	class DequeSort {
	private:
		DequeSort(const DequeSort& other);
		DequeSort& operator=(const DequeSort& other);
		std::deque<int> _argv;
		std::deque<int> _main;
		std::deque<int> _pending;
		std::deque<size_t> _jacobNumbers;
		std::deque<size_t> _insertOrder;
	public:
		DequeSort();
		~DequeSort();
		size_t getMainSize();
		void checkArg(const std::string& arg);
		void changePairPos(int low, int high);
		int binarySearchWithPair(int high);
		void eraseAndInsert(int high, int insertPos);
		void makePair(int idx);
		void doSort();
		void splitMainPending();
		size_t getJacobsthalNumber(size_t n);
		void makeJacobsthalNumbers();
		void setInsertOrder();
		void insertionSort();
		size_t binarySearch(int insertNum);
		void beforePrint();
		void afterPrint();
		void checkSort();
	};
};

#endif