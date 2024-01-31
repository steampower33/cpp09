#include "PmergeMe.hpp"

int main(int argc, char** argv) {
	if (argc < 2)
	{
		std::cout << "Error" << std::endl;
		return 1;
	}

	PmergeMe pm;
	try
	{
		argv++;
		while (*argv) {
			pm.checkArg(*argv);
			argv++;
		}
		pm.doSort();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	return 0;
}