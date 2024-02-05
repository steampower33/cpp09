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

PmergeMe::VecterSort::VecterSort() {};

PmergeMe::VecterSort::~VecterSort() {};

PmergeMe::VecterSort::VecterSort(const VecterSort& other) {
	*this = other;
}

PmergeMe::VecterSort& PmergeMe::VecterSort::operator=(const VecterSort& other) {
	if (this != &other) {
		_argv = other._argv;
		_main = other._main;
		_pending = other._pending;
		_jacobNumbers = other._jacobNumbers;
		_insertOrder = other._insertOrder;
	}
	return *this;
}

size_t PmergeMe::VecterSort::getMainSize() {
	return _main.size();
}

void PmergeMe::VecterSort::checkArg(const std::string& arg) {
	char* endptr = NULL;
	double num = strtod(arg.c_str(), &endptr);
	if (*endptr == '\0' && num > 0 && num == static_cast<int>(num))
		_argv.push_back(num);
	else
		throw std::runtime_error("Error");
}

void PmergeMe::VecterSort::changePairPos(int low, int high) {
	int tmp;

	if (_argv[low] < _argv[high]) {
		tmp = _argv[low];
		_argv[low] = _argv[high];
		_argv[high] = tmp;
	}
	// std::cout << "low high" << std::endl;
	// std::cout << _argv[low] << " " << _argv[high] << " " << std::endl;
}

int PmergeMe::VecterSort::binarySearchWithPair(int high) {
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

void PmergeMe::VecterSort::eraseAndInsert(int high, int insertPos) {
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

void PmergeMe::VecterSort::makePair(int idx) {
	int insertPos;

	if (idx < 0) return ;
	makePair(idx - 2);
	changePairPos(idx - 1, idx);
	insertPos = binarySearchWithPair(idx / 2);
	// std::cout << "insertPos" << std::endl;
	// std::cout << "(" << _argv[idx - 1] << ", "<< _argv[idx] << ") " << insertPos * 2 << std::endl;
	eraseAndInsert(idx, insertPos);
}

void PmergeMe::VecterSort::splitMainPending() {
	size_t argvSize = _argv.size() / 2;
	size_t idx = 0;
	while (argvSize) {
		_main.push_back(_argv[idx]);
		_pending.push_back(_argv[idx + 1]);
		idx += 2;
		argvSize--;
	}
}

size_t PmergeMe::VecterSort::getJacobsthalNumber(size_t n)
{
	if (n == 0)
		return (0);
	else if (n == 1)
		return (1);
	size_t num = getJacobsthalNumber(n - 1) + 2 * getJacobsthalNumber(n - 2);
	return (num);
}

void PmergeMe::VecterSort::makeJacobsthalNumbers() {
	size_t jacobsthalNum = 0;
	size_t pendingSize = _pending.size();
	size_t pendingIdx = 2;

	while ((jacobsthalNum = getJacobsthalNumber(pendingIdx)) < pendingSize) {
		_jacobNumbers.push_back(jacobsthalNum);
		pendingIdx++;
	}
}

void PmergeMe::VecterSort::setInsertOrder() {
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

size_t PmergeMe::VecterSort::binarySearch(int insertNum) {
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

void PmergeMe::VecterSort::insertionSort() {
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

void PmergeMe::VecterSort::beforePrint() {
	size_t beforeSize = _argv.size();
	std::cout << "Before: ";
	for (size_t i = 0; i < beforeSize; ++i) {
		std::cout << _argv[i] << " ";
	}
	std::cout << std::endl;
}

void PmergeMe::VecterSort::afterPrint() {
	size_t mainSize = _main.size();
	std::cout << "After: \t";
	for (size_t i = 0; i < mainSize; ++i) {
		std::cout << _main[i] << " ";
	}
	std::cout << std::endl;
}

void PmergeMe::VecterSort::doSort() {
	size_t argvSize = (_argv.size() / 2) * 2;

	makePair(argvSize - 1);
	// std::cout << "<<< _argv >>>" << std::endl;
	// for (size_t i = 0; i < _argv.size(); ++i) {
	// 	std::cout << _argv[i] << " ";
	// }
	// std::cout << std::endl;
	splitMainPending();
	// std::cout << "<<< Main And Pending >>>" << std::endl;
	// std::cout << "main: ";
	// for (size_t i = 0; i < _main.size(); ++i)
	// 	std::cout << _main[i] << " ";
	// std::cout << std::endl;
	// std::cout << "pending: ";
	// for (size_t i = 0; i < _pending.size(); ++i)
	// 	std::cout << _pending[i] << " ";
	// std::cout << std::endl;
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
	checkSort();
}

void PmergeMe::VecterSort::checkSort() {
	size_t mainSize = _main.size();

	for (size_t i = 0; i < mainSize - 1; i++) {
		if (_main[i] <= _main[i + 1])
			continue;
		else
			throw std::runtime_error("Error");
	}
}

// DequeSort------------------------------------------------------------------------------------------------------------------------------------

PmergeMe::DequeSort::DequeSort() {};

PmergeMe::DequeSort::~DequeSort() {};

PmergeMe::DequeSort::DequeSort(const DequeSort& other) {
	*this = other;
}

PmergeMe::DequeSort& PmergeMe::DequeSort::operator=(const DequeSort& other) {
	if (this != &other) {
		_argv = other._argv;
		_main = other._main;
		_pending = other._pending;
		_jacobNumbers = other._jacobNumbers;
		_insertOrder = other._insertOrder;
	}
	return *this;
}

size_t PmergeMe::DequeSort::getMainSize() {
	return _main.size();
}

void PmergeMe::DequeSort::checkArg(const std::string& arg) {
	char* endptr = NULL;
	double num = strtod(arg.c_str(), &endptr);
	if (*endptr == '\0' && num > 0 && num == static_cast<int>(num))
		_argv.push_back(num);
	else
		throw std::runtime_error("Error");
}

void PmergeMe::DequeSort::changePairPos(int low, int high) {
	int tmp;

	if (_argv[low] < _argv[high]) {
		tmp = _argv[low];
		_argv[low] = _argv[high];
		_argv[high] = tmp;
	}
	// std::cout << "low high" << std::endl;
	// std::cout << _argv[low] << " " << _argv[high] << " " << std::endl;
}

int PmergeMe::DequeSort::binarySearchWithPair(int high) {
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

void PmergeMe::DequeSort::eraseAndInsert(int high, int insertPos) {
	std::deque<int>::iterator it;

	int left = _argv[high - 1];
	int right = _argv[high];
	_argv.erase(_argv.begin() + high - 1, _argv.begin() + high + 1);
    _argv.insert(_argv.begin() + insertPos * 2, right);
    _argv.insert(_argv.begin() + insertPos * 2, left);
	// std::cout << "_argv" << std::endl;
	// for (size_t i = 0; i < _argv.size(); ++i) {
	// 	std::cout << _argv[i] << " ";
	// }
	// std::cout << std::endl;
}

void PmergeMe::DequeSort::makePair(int idx) {
	int insertPos;

	if (idx < 0) return ;
	makePair(idx - 2);
	changePairPos(idx - 1, idx);
	insertPos = binarySearchWithPair(idx / 2);
	// std::cout << "insertPos" << std::endl;
	// std::cout << "(" << _argv[idx - 1] << ", "<< _argv[idx] << ") " << insertPos * 2 << std::endl;
	eraseAndInsert(idx, insertPos);
}

void PmergeMe::DequeSort::splitMainPending() {
	size_t argvSize = _argv.size() / 2;
	size_t idx = 0;
	while (argvSize) {
		_main.push_back(_argv[idx]);
		_pending.push_back(_argv[idx + 1]);
		idx += 2;
		argvSize--;
	}
}

size_t PmergeMe::DequeSort::getJacobsthalNumber(size_t n)
{
	if (n == 0)
		return (0);
	else if (n == 1)
		return (1);
	size_t num = getJacobsthalNumber(n - 1) + 2 * getJacobsthalNumber(n - 2);
	return (num);
}

void PmergeMe::DequeSort::makeJacobsthalNumbers() {
	size_t jacobsthalNum = 0;
	size_t pendingSize = _pending.size();
	size_t pendingIdx = 2;

	while ((jacobsthalNum = getJacobsthalNumber(pendingIdx)) < pendingSize) {
		_jacobNumbers.push_back(jacobsthalNum);
		pendingIdx++;
	}
}

void PmergeMe::DequeSort::setInsertOrder() {
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

size_t PmergeMe::DequeSort::binarySearch(int insertNum) {
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

void PmergeMe::DequeSort::insertionSort() {
	int insertNum;
	size_t insertPosition;
	std::deque<int>::iterator insertMainPostionIter;
	std::deque<size_t>::iterator iob = _insertOrder.begin();
	std::deque<size_t>::iterator ioe = _insertOrder.end();

	for (std::deque<size_t>::iterator iter = iob; iter < ioe; ++iter) {
		insertNum = _pending[*iter - 1];
		insertPosition = binarySearch(insertNum);
		_main.insert(_main.begin() + insertPosition, insertNum);
	}
	if (_argv.size() > 1 && _argv.size() % 2 == 1) {
		insertNum = _argv[_argv.size() - 1];
		insertPosition = binarySearch(insertNum);
		_main.insert(_main.begin() + insertPosition, insertNum);
	}
	if (_argv.size() == 1)
		_main.push_back(_argv[0]);
}

void PmergeMe::DequeSort::beforePrint() {
	size_t beforeSize = _argv.size();
	std::cout << "Before: ";
	for (size_t i = 0; i < beforeSize; ++i) {
		std::cout << _argv[i] << " ";
	}
	std::cout << std::endl;
}

void PmergeMe::DequeSort::afterPrint() {
	size_t mainSize = _main.size();
	std::cout << "After: \t";
	for (size_t i = 0; i < mainSize; ++i) {
		std::cout << _main[i] << " ";
	}
	std::cout << std::endl;
}

void PmergeMe::DequeSort::doSort() {
	size_t argvSize = (_argv.size() / 2) * 2;

	makePair(argvSize - 1);
	// std::cout << "<<< _argv >>>" << std::endl;
	// for (size_t i = 0; i < _argv.size(); ++i) {
	// 	std::cout << _argv[i] << " ";
	// }
	// std::cout << std::endl;
	splitMainPending();
	// std::cout << "<<< Main And Pending >>>" << std::endl;
	// std::cout << "main: ";
	// for (size_t i = 0; i < _main.size(); ++i)
	// 	std::cout << _main[i] << " ";
	// std::cout << std::endl;
	// std::cout << "pending: ";
	// for (size_t i = 0; i < _pending.size(); ++i)
	// 	std::cout << _pending[i] << " ";
	// std::cout << std::endl;
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
	checkSort();
}

void PmergeMe::DequeSort::checkSort() {
	size_t mainSize = _main.size();

	for (size_t i = 0; i < mainSize - 1; i++) {
		if (_main[i] <= _main[i + 1])
			continue;
		else {
			throw std::runtime_error("Error");
		}
	}
}