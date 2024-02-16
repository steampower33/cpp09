#include "PmergeMe.hpp"

int main(int argc, char** argv) {
	if (argc < 2)
	{
		std::cout << "Error" << std::endl;
		return 1;
	}

	char** argvAddress;
	PmergeMe pmerge;
	PmergeMe::VectorSort vs;
	PmergeMe::DequeSort ds;
	try
	{
		// Print Before Argv
		argvAddress = argv;
		argvAddress++;
		pmerge.checkArg(argvAddress);
		pmerge.argvPrint();

		// Start Vector Sort
		argvAddress = argv;
		argvAddress++;
		vs.doSort(argvAddress);

		// Start Deque Sort
		argvAddress = argv;
		argvAddress++;
		ds.doSort(argvAddress);
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << '\n';
		return 1;
	}
	return 0;
}