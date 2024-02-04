#include "PmergeMe.hpp"

int main(int argc, char** argv) {
	if (argc < 2)
	{
		std::cout << "Error" << std::endl;
		return 1;
	}

	char** argvAddress;
	clock_t start, end;
	PmergeMe pmerge;
	PmergeMe::VecterSort vs;
	PmergeMe::DequeSort ds;
	try
	{
		// Pinrt Argv
		argvAddress = argv;
		argvAddress++;
		while (*argvAddress) {
			pmerge.checkArg(*argvAddress);
			argvAddress++;
		}
		// pmerge.argvPrint();

		// Start Vector Sort
		start = clock();
		argvAddress = argv;
		argvAddress++;
		while (*argvAddress) {
			vs.checkArg(*argvAddress);
			argvAddress++;
		}
		vs.doSort();
		end = clock();
		// vs.afterPrint();
    	std::cout << "Time to process a range of " << vs.getMainSize() << " elements with std::vector : " << static_cast<float>(end - start) * 1000.0 / CLOCKS_PER_SEC << " us" << std::endl;

		// Start Deque Sort
		start = clock();
		argvAddress = argv;
		argvAddress++;
		while (*argvAddress) {
			ds.checkArg(*argvAddress);
			argvAddress++;
		}
		ds.doSort();
		end = clock();
    	std::cout << "Time to process a range of " << ds.getMainSize() << " elements with std::deque  : " << static_cast<float>(end - start) * 1000.0 / CLOCKS_PER_SEC << " us" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << '\n';
		return 1;
	}
	return 0;
}