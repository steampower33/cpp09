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

// void PmergeMe::VectorSort::beforePrint() {
// 	size_t beforeSize = _argv.size();
// 	std::cout << "Before: ";
// 	for (size_t i = 0; i < beforeSize; ++i) {
// 		std::cout << _argv[i] << " ";
// 	}
// 	std::cout << std::endl;
// }

// void PmergeMe::VectorSort::afterPrint() {
// 	size_t mainSize = _main.size();
// 	std::cout << "After: \t";
// 	for (size_t i = 0; i < mainSize; ++i) {
// 		std::cout << _main[i] << " ";
// 	}
// 	std::cout << std::endl;
// }

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
		if (insertNum == _main.at(mid)[0])
			return (mid);
		if (insertNum < _main.at(mid)[0])
			high = mid - 1;
		else
			low = mid + 1;
	}
	if (insertNum > _main.at(mid)[0])
		return (mid + 1);
	else
		return (mid);
}

void PmergeMe::VectorSort::insertBInA(size_t unitSize) {
	int insertNum;
	size_t insertPosition;
	std::vector<int>::iterator insertMainPostionIter;
	std::vector<size_t>::iterator iob = _insertOrder.begin();
	std::vector<size_t>::iterator ioe = _insertOrder.end();

	for (std::vector<size_t>::iterator iter = iob; iter < ioe; ++iter) {
		insertNum = _pending[*iter - 1][0];
		insertPosition = binarySearch(insertNum);

		std::cout << "insertNum: " << insertNum << " insertPosition: " << insertPosition << " " << std::endl;
		_main.insert(_main.begin() + insertPosition, _pending[*iter - 1]);
	}
	if ((_argv.size() / unitSize) % 2 == 1) {

	}
}

void PmergeMe::VectorSort::recur(size_t size) {
	size_t i;

	// Swap Main Pending
	std::cout << "--------" << std::endl;
	std::cout << "size => " << size << std::endl;
	size_t loopSize = (_argv.size() / size) * size;
	for (i = 0; i < loopSize; i += size) {
		if (_argv[i] < _argv[i + size / 2]) {
			for (size_t j = 0; j < size / 2; j++)
				_argv.insert(_argv.begin() + i, *(_argv.begin() + i + size - 1));
			for (size_t j = 0; j < size / 2; j++)
				_argv.erase(_argv.begin() + i + size);
		}
	}
	for (size_t l = 0; l < _argv.size(); l++) {
		if (l % size == 0)
			std::cout << "| ";
		std::cout << _argv[l] << " ";
	}
	std::cout << std::endl;

	// Escape Condition
	if (_argv.size() / size == 0) return ;
	
	// Recursion
	recur(size * 2);

	std::cout << "size => " << size << std::endl;
	std::cout << "loopSize => " << loopSize << std::endl;

	// Split Main & Pending Index
	for (i = 0; i < loopSize; i += size) {
		std::vector<int> m;
		for (size_t j = i; j < i + size / 2; j++)
			m.push_back(_argv[j]);
		_main.push_back(m);

		std::vector<int> p;
		for (size_t j = i + size / 2; j < i + size; j++)
			p.push_back(_argv[j]);
		_pending.push_back(p);
	}
	if ((_argv.size() / (size / 2)) % 2 == 1) {
		std::vector<int> l;
		for (size_t j = i; j < i + size / 2; j++)
			l.push_back(_argv[j]);
		_pending.push_back(l);
	}
	
	// Check Main & Pending
	std::cout << "m: ";
	for (size_t size = 0; size < _main.size(); size++) {
		std::cout << _main[size][0] << " ";
	}
	std::cout << std::endl;
	std::cout << "p: ";
	for (size_t size = 0; size < _pending.size(); size++) {
		std::cout << _pending[size][0] << " ";
	}
	std::cout << std::endl;


	// Merge Insertion Sort

	// Make Jacobsthal
	makeJacobsthalNumbers();

	std::cout << "Jacobsthal" << std::endl;
	for (size_t i = 0; i < _jacobNumbers.size(); i++) {
		std::cout << _jacobNumbers[i] << " ";
	}
	std::cout << std::endl;

	// Set InsertOrder
	setInsertOrder();

	std::cout << "InsertOrder" << std::endl;
	for (size_t i = 0; i < _insertOrder.size(); i++) {
		std::cout << _insertOrder[i] << " ";
	}
	std::cout << std::endl;

	// insert
	insertBInA(size / 2);

	if ((_argv.size() / (size / 2)) * (size / 2) < _argv.size()) {
		std::vector<int> l;
		for (size_t j = (_argv.size() / (size / 2)) * (size / 2); j < _argv.size(); j++)
			l.push_back(_argv[j]);
		_main.push_back(l);
	}

	std::cout << "argv => " << std::endl;
	for (size_t i = 0; i < _main.size(); i++) {
		for (size_t j = 0; j < _main[i].size(); j++) {
			_argv[i * size / 2 + j] = _main[i][j];
			std::cout << _argv[i * size / 2 + j] << " ";
		}
		std::cout << "| ";
	}
	std::cout << std::endl;

	// Clear Main & Pending
	_main.clear();
	_pending.clear();
	_jacobNumbers.clear();
	_insertOrder.clear();
	return ;
}

void PmergeMe::VectorSort::doSort() {
	std::cout << "argv size => " << _argv.size() << std::endl;
	recur(2);
}
