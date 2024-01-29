#include "BitcoinExchange.hpp"

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "Error: The number of parameters is incorrect" << std::endl;
		return 1;
	}

	std::ifstream inputFile(argv[1]);
	if (!inputFile.is_open()) {
		std::cout << "Error : Csv Does Not Open" << std::endl;
		return 1;
	}

	BitcoinExchange be;
	try
	{
		be.checkDataCsv();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

    // while (std::getline(inputFile, line)) {
    //     if (line.find("date | value") != std::string::npos) {
    //         continue; // Skip header line
    //     }

    //     std::istringstream iss(line);
    //     std::string date;
	// 	std::string pipe;
    //     float value;

    //     if (!(iss >> date >> pipe >> value) || iss.peek() != EOF) {
    //         // Error already handled during the first pass
    //         continue;
    //     }

    //     float bitcoinPrice = be.getPrice(date);
    //     if (bitcoinPrice >= 0) {
    //         float result = value * bitcoinPrice;
    //         std::cout << date << " => " << value << " = " << result << std::endl;
    //     } else {
    //         std::cerr << "Error: date not found => " << date << std::endl;
    //     }
    // }
	return 0;
}