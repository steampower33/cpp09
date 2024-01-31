#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other) {
	*this = other;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
	if (this != &other) {
		_argv = other._argv;
	}
	return *this;
}

PmergeMe::~PmergeMe() {}

void PmergeMe::checkArg(const std::string& arg) {
	char* endptr = NULL;
	double num = strtod(arg.c_str(), &endptr);
	if (*endptr == '\0' && num > 0 && num == static_cast<int>(num))
		_argv.push_back(num);
	else
		throw std::runtime_error("Error");
}

void PmergeMe::makePair() {
	size_t pairHalfSize = _argv.size() / 2;
	size_t idx = 0;
	int smaller;
	int bigger;
	while (pairHalfSize) {
		if (_argv[idx + 1] && _argv[idx] < _argv[idx + 1]) {
			smaller = _argv[idx];
			bigger = _argv[idx + 1];
			idx += 2;
		}
		else if (_argv[idx + 1] && _argv[idx] >= _argv[idx + 1]) {
			smaller = _argv[idx + 1];
			bigger = _argv[idx];
			idx += 2;
		}
		_pair.push_back(std::make_pair(bigger, smaller));
		pairHalfSize--;
	}
}

void PmergeMe::merge(size_t low, size_t mid, size_t high) {
	size_t left = low;
	size_t right = mid + 1;
	size_t mergePairIdx = low;
	std::vector< std::pair<int, int> > _mergePair(_pair.size());

	while (left <= mid && right <= high) {
		if (_pair[left].first <= _pair[right].first)
			_mergePair[mergePairIdx++] = _pair[left++];
		else
			_mergePair[mergePairIdx++] = _pair[right++];
	}
	if (left > mid) {
		for (size_t r = right; r <= high; r++) {
			_mergePair[mergePairIdx++] = _pair[r];
		}
	} else {
		for (size_t l = left; l <= mid; l++) {
			_mergePair[mergePairIdx++] = _pair[l];
		}
	}
	for (size_t i = low; i <= high; ++i) {
		_pair[i] = _mergePair[i];
	}
}


void PmergeMe::mergeSort(size_t low, size_t high) {
	int mid = (low + high) / 2;

	if (low >= high) return ;
	mergeSort(low, mid);
	mergeSort(mid + 1, high);
	merge(low, mid, high);
}

void PmergeMe::splitMainPending() {
	size_t pairSize = _pair.size();
	for (size_t i = 0; i < pairSize; ++i) {
		_main.push_back(_pair[i].first);
		_pending.push_back(_pair[i].second);
	}
}

size_t PmergeMe::getJacobsthalNumber(size_t n)
{
	if (n == 0)
		return (0);
	else if (n == 1)
		return (1);
	size_t num = getJacobsthalNumber(n - 1) + 2 * getJacobsthalNumber(n - 2);
	return (num);
}

void PmergeMe::makeJacobsthalNumbers() {
	size_t jacobsthalNum = 0;
	size_t pendingSize = _pending.size();
	size_t pendingIdx = 0;

	while ((jacobsthalNum = getJacobsthalNumber(pendingIdx)) < pendingSize) {
		_jacobNumbers.push_back(jacobsthalNum);
		pendingIdx++;
	}
}

void PmergeMe::setInsertOrder() {
	size_t jacobSize = _jacobNumbers.size();
	size_t i = 2;
	size_t value;
	size_t pos;
	size_t last_pos = 1;

    while (i < jacobSize)
	{
		value = _jacobNumbers[i];
		_insertOrder.push_back(value);
		pos = value - 1;
		while (pos > last_pos)
		{
			_insertOrder.push_back(pos);
			pos--;
		}
		last_pos = value;
		++i;
	}
	value = _pending.size();
	while (value > last_pos)
	{
		_insertOrder.push_back(value);
		value--;
	}
}

size_t PmergeMe::binarySearch(int insertNum) {
	int low = 0;
	int mid;
	int high = _main.size() - 1;

	while (low <= high)
	{
		mid = low + (high - low) / 2;
		if (insertNum == _main.at(mid))
			return (mid);
		if (insertNum < _main.at(mid))
			high = mid - 1;
		else
			low = mid + 1;
	}
	if (insertNum > _main.at(mid))
		return (mid + 1);
	else
		return (mid);
}

void PmergeMe::insertionSort() {
	int insertNum;
	size_t insertPosition;
	std::vector<int>::iterator insertMainPostionIter;
	std::vector<size_t>::iterator iob = _insertOrder.begin();
	std::vector<size_t>::iterator ioe = _insertOrder.end();

	for (std::vector<size_t>::iterator iter = iob; iter < ioe; ++iter) {
		insertNum = _pending[*iter - 1];
		insertPosition = binarySearch(insertNum);
		insertMainPostionIter = _main.begin() + insertPosition;
		_main.insert(insertMainPostionIter, insertNum);
	}
	if (_argv.size() % 2 == 1) {
		insertNum = _argv[_argv.size() - 1];
		insertPosition = binarySearch(insertNum);
		insertMainPostionIter = _main.begin() + insertPosition;
		_main.insert(insertMainPostionIter, insertNum);
	}

}

void PmergeMe::beforePrint() {
	size_t beforeSize = _argv.size();
	std::cout << "Before: ";
	for (size_t i = 0; i < beforeSize; ++i) {
		std::cout << _argv[i] << " ";
	}
	std::cout << std::endl;
}

void PmergeMe::afterPrint() {
	size_t mainSize = _main.size();
	std::cout << "After: \t";
	for (size_t i = 0; i < mainSize; ++i) {
		std::cout << _main[i] << " ";
	}
	std::cout << std::endl;
}

void PmergeMe::doSort() {
	beforePrint();
	clock_t start = clock();
	makePair();
	// std::cout << "<<< Pair >>>" << std::endl;
	// size_t pairSize = _pair.size();
	// for (size_t i = 0; i < pairSize; ++i) {
	// 	std::cout << _pair[i].first << " " << _pair[i].second << std::endl;
	// }
	mergeSort(0, _pair.size() - 1);
	// std::cout << "<<< MergePair >>>" << std::endl;
	// for (size_t i = 0; i < _pair.size(); ++i) {
	// 	std::cout << _pair[i].first << " " << _pair[i].second << std::endl;
	// }
	splitMainPending();
	// std::cout << "<<< Main And Pending >>>" << std::endl;
	// for (size_t i = 0; i < _main.size(); ++i)
	// 	std::cout << "main: " << _main[i] << ", pending: " << _pending[i] << std::endl;
	makeJacobsthalNumbers();
	// std::cout << "<<< Jacobsthal Numbers >>>" << std::endl;
	// for (size_t i = 0; i < _jacobNumbers.size(); ++i) {
	// 	std::cout << _jacobNumbers[i] << " ";
	// }
	// std::cout << std::endl;
	setInsertOrder();
	// std::cout << "<<< Insert Order >>>" << std::endl;
	// for (size_t i = 0; i < _insertOrder.size(); ++i)
	// 	std::cout << _insertOrder[i] << " ";
	// std::cout << std::endl;
	insertionSort();
	// std::cout << "<<< Sort Result >>>" << std::endl;
	// for (size_t i = 0; i < _main.size(); ++i)
	// 	std::cout << _main[i] << " ";
	// std::cout << std::endl;
	clock_t end = clock();
	afterPrint();
	double duration = (static_cast<double>(end - start) * 1000000.0) / CLOCKS_PER_SEC;
    std::cout << "Time to process a range of " << _argv.size() << " elements with std::vector : " << duration << " us" << std::endl;
}