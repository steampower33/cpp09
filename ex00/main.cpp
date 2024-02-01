#include "BitcoinExchange.hpp"

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "Error: The number of parameters is incorrect" << std::endl;
		return 1;
	}

	std::ifstream inputFile(argv[1]);
	if (!inputFile.is_open()) {
		std::cout << "Error : inputfile[" << argv[1] << "] Does Not Open" << std::endl;
		return 1;
	}

	BitcoinExchange be;
	try
	{
		be.checkDataCsv();
		be.checkInputFile(argv[1]);
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << '\n';
		return 1;
	}
	return 0;
}