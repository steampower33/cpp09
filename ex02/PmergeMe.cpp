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

void PmergeMe::checkArg(char** argv) {
	size_t idx = 0;

	while (argv[idx]) {
		char* endptr = NULL;
		double num = strtod(argv[idx], &endptr);
		if (*endptr == '\0' && num > 0 && num == static_cast<int>(num))
			_argv.push_back(num);
		else
			throw std::runtime_error("Error: Argument is not Valid");
		idx++;
	}
}

void PmergeMe::argvPrint() {
	size_t beforeSize = _argv.size();
	std::cout << "Before: ";
	for (size_t i = 0; i < beforeSize; i++) {
		std::cout << _argv[i] << " ";
	}
	std::cout << std::endl;
}

// VectorSort------------------------------------------------------------------------------------------------------------------------------------

PmergeMe::VectorSort::VectorSort() {};

PmergeMe::VectorSort::~VectorSort() {};

PmergeMe::VectorSort::VectorSort(const VectorSort& other) {
	_argv = other._argv;
	_main = other._main;
	_pending = other._pending;
	_jacobNumbers = other._jacobNumbers;
	_insertOrder = other._insertOrder;
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

void PmergeMe::VectorSort::checkArg(char** argv) {
	size_t idx = 0;

	while (argv[idx]) {
		char* endptr = NULL;
		double num = strtod(argv[idx], &endptr);
		if (*endptr == '\0' && num > 0 && num == static_cast<int>(num))
			_argv.push_back(num);
		else
			throw std::runtime_error("Error: Argument is not Valid");
		idx++;
	}
}

void PmergeMe::VectorSort::doSort(char** argv) {
	clock_t start, end;

	start = clock();
	checkArg(argv);
	recur(2);
	checkSort();
	end = clock();
	afterPrint();
    std::cout << "Time to process a range of " << getArgvSize() << " elements with std::vector : " << static_cast<float>(end - start) * 1000.0 / CLOCKS_PER_SEC << " us" << std::endl;
}

void PmergeMe::VectorSort::recur(size_t size) {
	size_t loopSize = (_argv.size() / size) * size;
	if (_argv.size() / size == 0) return ;
	swapMainPending(size, loopSize);
	
	recur(size * 2);
	
	splitMainPending(size, loopSize);
	makeJacobsthalNumbers();
	setInsertOrder();
	insertBInA(size / 2);
	moveMainToArgv(size);
	clearVector();
}

void PmergeMe::VectorSort::swapMainPending(size_t size, size_t loopSize) {
	for (size_t i = 0; i < loopSize; i += size) {
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
}

void PmergeMe::VectorSort::splitMainPending(size_t size, size_t loopSize) {
	size_t i;
	
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

	std::cout << "size => " << size << std::endl;
	std::cout << "loopSize => " << loopSize << std::endl;
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
}

void PmergeMe::VectorSort::makeJacobsthalNumbers() {
	size_t jacobsthalNum = 0;
	size_t pendingSize = _pending.size();
	size_t pendingIdx = 2;

	while ((jacobsthalNum = getJacobsthalNumber(pendingIdx)) < pendingSize) {
		_jacobNumbers.push_back(jacobsthalNum);
		pendingIdx++;
	}

	std::cout << "Jacobsthal" << std::endl;
	for (size_t i = 0; i < _jacobNumbers.size(); i++) {
		std::cout << _jacobNumbers[i] << " ";
	}
	std::cout << std::endl;
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

void PmergeMe::VectorSort::setInsertOrder() {
	size_t jacobSize = _jacobNumbers.size();
	size_t jacobNum;
	size_t order;
	size_t jacobIdx = 0;
	size_t lastJacob = 0;

    while (jacobIdx < jacobSize)
	{
		jacobNum = _jacobNumbers[jacobIdx];
		_insertOrder.push_back(jacobNum);
		order = jacobNum - 1;
		while (order > lastJacob)
		{
			_insertOrder.push_back(order);
			order--;
		}
		lastJacob = jacobNum;
		jacobIdx++;
	}
	order = _pending.size();
	while (order > lastJacob)
	{
		_insertOrder.push_back(order);
		order--;
	}

	std::cout << "InsertOrder" << std::endl;
	for (size_t i = 0; i < _insertOrder.size(); i++) {
		std::cout << _insertOrder[i] << " ";
	}
	std::cout << std::endl;
}

void PmergeMe::VectorSort::insertBInA(size_t unitSize) {
	size_t orderSize = _insertOrder.size();
	int insertNum;
	size_t insertPosition = 0;
	size_t searchSize = 0;
	size_t jacobIdx = 0;
	size_t firstMainSize = _main.size();

	for (size_t i = 0; i < orderSize; i++) {
		insertNum = _pending[_insertOrder[i] - 1][0];
		if (firstMainSize < _insertOrder[i])
			insertPosition = binarySearch(insertNum, searchSize + firstMainSize - 1);
		else
			insertPosition = binarySearch(insertNum, searchSize + _insertOrder[i] - 1);
		_main.insert(_main.begin() + insertPosition, _pending[_insertOrder[i] - 1]);
		if (jacobIdx + 1 < _jacobNumbers.size() && _insertOrder[i] == _jacobNumbers[jacobIdx + 1]) {
			searchSize = _jacobNumbers[jacobIdx + 1];
			jacobIdx++;
		}
	}
	if ((_argv.size() / unitSize) * unitSize < _argv.size()) {
		std::vector<int> l;
		for (size_t j = (_argv.size() / unitSize) * unitSize; j < _argv.size(); j++)
			l.push_back(_argv[j]);
		_main.push_back(l);
	}
}

size_t PmergeMe::VectorSort::binarySearch(int insertNum, int high) {
	int low = 0;
	int mid;

	while (low <= high)
	{
		mid = low + (high - low) / 2;
		if (_main.at(mid)[0] < insertNum)
			low = mid + 1;
		else
			high = mid - 1;
	}
	if (_main.at(mid)[0] < insertNum)
		return (mid + 1);
	else
		return (mid);
}

void PmergeMe::VectorSort::moveMainToArgv(size_t size) {
	for (size_t i = 0; i < _main.size(); i++) {
		for (size_t j = 0; j < _main[i].size(); j++)
			_argv[i * size / 2 + j] = _main[i][j];
	}
}

void PmergeMe::VectorSort::clearVector() {
	_main.clear();
	_pending.clear();
	_jacobNumbers.clear();
	_insertOrder.clear();
}

void PmergeMe::VectorSort::checkSort() {
	size_t argvSize = _argv.size();

	for (size_t i = 0; i < argvSize - 1; i++) {
		if (_argv[i] > _argv[i + 1])
			throw std::runtime_error("Error: Vector is not sorted");
	}
}

void PmergeMe::VectorSort::afterPrint() {
	size_t argvSize = _argv.size();
	std::cout << "After: \t";
	for (size_t i = 0; i < argvSize; i++) {
		std::cout << _argv[i] << " ";
	}
	std::cout << std::endl;
}

size_t PmergeMe::VectorSort::getArgvSize() {
	return _argv.size();
}