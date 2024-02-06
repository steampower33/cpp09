#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other) {
	*this = other;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
	if (this != &other) {
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

void PmergeMe::argvPrint() {
	size_t beforeSize = _argv.size();
	std::cout << "Before: ";
	for (size_t i = 0; i < beforeSize; ++i) {
		std::cout << _argv[i] << " ";
	}
	std::cout << std::endl;
}

// VectorSort------------------------------------------------------------------------------------------------------------------------------------

PmergeMe::VectorSort::VectorSort() {};

PmergeMe::VectorSort::~VectorSort() {};

PmergeMe::VectorSort::VectorSort(const VectorSort& other) {
	*this = other;
}

PmergeMe::VectorSort& PmergeMe::VectorSort::operator=(const VectorSort& other) {
	if (this != &other) {
		_argv = other._argv;
		_main = other._main;
		_pending = other._pending;
		_jacobNumbers = other._jacobNumbers;
		_insertOrder = other._insertOrder;
	}
	return *this;
}

size_t PmergeMe::VectorSort::getMainSize() {
	return _main.size();
}

void PmergeMe::VectorSort::checkArg(const std::string& arg) {
	char* endptr = NULL;
	double num = strtod(arg.c_str(), &endptr);
	if (*endptr == '\0' && num > 0 && num == static_cast<int>(num))
		_argv.push_back(num);
	else
		throw std::runtime_error("Error");
}

int PmergeMe::VectorSort::binarySearchWithPair(int high) {
	int low = 0;
	int mid;
	int insertNum = _argv[high * 2];

	while (low <= high)
	{
		mid = low + (high - low) / 2;
		if (insertNum == _argv.at(mid * 2))
			return (mid);
		if (insertNum < _argv.at(mid * 2))
			high = mid - 1;
		else
			low = mid + 1;
	}
	if (insertNum > _argv.at(mid * 2))
		return (mid + 1);
	else
		return (mid);
}

void PmergeMe::VectorSort::eraseAndInsert(int high, int insertPos) {
	std::vector<int>::iterator it;

	int left = _argv[high - 1];
	int right = _argv[high];
	it = _argv.begin() + high - 1;
	_argv.erase(it);
	_argv.erase(it);
	it = _argv.begin() + insertPos * 2;
	_argv.insert(it, right);
	_argv.insert(it, left);
	// std::cout << "_argv" << std::endl;
	// for (size_t i = 0; i < _argv.size(); ++i) {
	// 	std::cout << _argv[i] << " ";
	// }
	// std::cout << std::endl;
}

size_t PmergeMe::VectorSort::getJacobsthalNumber(size_t n)
{
	if (n == 0)
		return (0);
	else if (n == 1)
		return (1);
	size_t num = getJacobsthalNumber(n - 1) + 2 * getJacobsthalNumber(n - 2);
	return (num);
}

void PmergeMe::VectorSort::makeJacobsthalNumbers() {
	size_t jacobsthalNum = 0;
	size_t pendingSize = _pending.size();
	size_t pendingIdx = 2;

	while ((jacobsthalNum = getJacobsthalNumber(pendingIdx)) < pendingSize) {
		_jacobNumbers.push_back(jacobsthalNum);
		pendingIdx++;
	}
}

void PmergeMe::VectorSort::setInsertOrder() {
	size_t jacobSize = _jacobNumbers.size();
	size_t jacobNum;
	size_t order;
	size_t jacobIdx = 0;
	size_t last_pos = 0;

    while (jacobIdx < jacobSize)
	{
		jacobNum = _jacobNumbers[jacobIdx];
		_insertOrder.push_back(jacobNum);
		order = jacobNum - 1;
		while (order > last_pos)
		{
			_insertOrder.push_back(order);
			--order;
		}
		last_pos = jacobNum;
		++jacobIdx;
	}
	order = _pending.size();
	while (order > last_pos)
	{
		_insertOrder.push_back(order);
		--order;
	}
}

size_t PmergeMe::VectorSort::binarySearch(int insertNum) {
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

void PmergeMe::VectorSort::insertionSort() {
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
	if (_argv.size() > 1 && _argv.size() % 2 == 1) {
		insertNum = _argv[_argv.size() - 1];
		insertPosition = binarySearch(insertNum);
		insertMainPostionIter = _main.begin() + insertPosition;
		_main.insert(insertMainPostionIter, insertNum);
	}
	if (_argv.size() == 1)
		_main.push_back(_argv[0]);
}

void PmergeMe::VectorSort::beforePrint() {
	size_t beforeSize = _argv.size();
	std::cout << "Before: ";
	for (size_t i = 0; i < beforeSize; ++i) {
		std::cout << _argv[i] << " ";
	}
	std::cout << std::endl;
}

void PmergeMe::VectorSort::afterPrint() {
	size_t mainSize = _main.size();
	std::cout << "After: \t";
	for (size_t i = 0; i < mainSize; ++i) {
		std::cout << _main[i] << " ";
	}
	std::cout << std::endl;
}

void PmergeMe::VectorSort::recur(size_t size) {
	size_t loopSize = (_argv.size() / 2) * 2;

	for (size_t i = 0; i < loopSize; i += size) {
		if (_argv[i] < _argv[i + size / 2]) {
			for (size_t j = 0; j < size / 2; ++j)
				_argv.insert(_argv.begin() + i, *(_argv.begin() + i + size - 1));
			for (size_t j = 0; j < size / 2; ++j)
				_argv.erase(_argv.begin() + i + size);
		}
	}
	std::cout << "-----argv-----" << std::endl;
	for (size_t i = 0; i < _argv.size(); ++i) {
		std::cout << _argv[i] << " ";
	}
	std::cout << std::endl;
	if (_argv.size() == size) return ;
	recur(size * 2);
}

void PmergeMe::VectorSort::doSort() {
	recur(2);
}

void PmergeMe::VectorSort::checkSort() {
	size_t mainSize = _main.size();

	for (size_t i = 0; i < mainSize - 1; i++) {
		if (_main[i] <= _main[i + 1])
			continue;
		else
			throw std::runtime_error("Error");
	}
}
