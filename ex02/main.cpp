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
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
}