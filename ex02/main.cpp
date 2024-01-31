#include "PmergeMe.hpp"

int main(int argc, char** argv) {
	if (argc < 2)
	{
		std::cout << "Error" << std::endl;
		return 1;
	}

	PmergeMe::VecterSort vs;
	PmergeMe::DequeSort ds;
	try
	{
		argv++;
		while (*argv) {
			vs.checkArg(*argv);
			ds.checkArg(*argv);
			argv++;
		}
		vs.doSort();
		ds.doSort();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	return 0;
}