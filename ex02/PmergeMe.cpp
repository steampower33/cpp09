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
	size_t pairSize = _argv.size() / 2;
	size_t idx = 0;
	int smaller;
	int bigger;
	while (pairSize) {
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
		pairSize--;
	}
	std::cout << "<<< Pair >>>" << std::endl;
	for (size_t i = 0; i < _pair.size(); i++) {
		std::cout << _pair[i].first << " " << _pair[i].second << std::endl;
	}
}

void PmergeMe::merge(int low, int mid, int high) {
	int left = low;
	int right = mid + 1;
	int mergePairIdx = low;
	std::vector< std::pair<int, int> > _mergePair(_pair.size());

	while (left <= mid && right <= high) {
		if (_pair[left].first <= _pair[right].first)
			_mergePair[mergePairIdx++] = _pair[left++];
		else
			_mergePair[mergePairIdx++] = _pair[right++];
	}
	if (left > mid) {
		for (int r = right; r <= high; r++) {
			_mergePair[mergePairIdx++] = _pair[r];
		}
	} else {
		for (int l = left; l <= mid; l++) {
			_mergePair[mergePairIdx++] = _pair[l];
		}
	}
	for (int i = low; i <= high; i++) {
		_pair[i] = _mergePair[i];
	}
}


void PmergeMe::mergeSort(int low, int high) {
	int mid = (low + high) / 2;

	if (low >= high) return ;
	mergeSort(low, mid);
	mergeSort(mid + 1, high);
	merge(low, mid, high);
}

void PmergeMe::splitMainPending() {
	for (size_t i = 0; i < _pair.size(); i++) {
		_main.push_back(_pair[i].first);
		_pending.push_back(_pair[i].second);
	}
}

int PmergeMe::getJacobsthalNumber(int n)
{
	if (n == 0)
		return (0);
	else if (n == 1)
		return (1);
	int i = getJacobsthalNumber(n - 1) + 2 * getJacobsthalNumber(n - 2);
	return (i);
}

void PmergeMe::makeJacobsthalNumbers() {
	size_t jacobsthalNum = 0;
	size_t pendingSize = _pending.size();
	int pendingIdx;

	pendingIdx = 0;
	while ((jacobsthalNum = getJacobsthalNumber(pendingIdx)) < pendingSize) {
		_jacobNumbers.push_back(jacobsthalNum);
		pendingIdx++;
	}
}

void PmergeMe::setInsertPosition() {
	makeJacobsthalNumbers();
	for (size_t i = 0; i < _jacobNumbers.size(); i++) {
		std::cout << _jacobNumbers[i] << " " << std::endl;
	}
}

void PmergeMe::pairSort() {
	mergeSort(0, _pair.size() - 1);
	std::cout << "<<< MergePair >>>" << std::endl;
	for (size_t i = 0; i < _pair.size(); i++) {
		std::cout << _pair[i].first << " " << _pair[i].second << std::endl;
	}
	splitMainPending();
	std::cout << "<<< Main And Pending >>>" << std::endl;
	for (size_t i = 0; i < _main.size(); i++)
		std::cout << "main: " << _main[i] << ", pending: " << _pending[i] << std::endl;
	setInsertPosition();
}