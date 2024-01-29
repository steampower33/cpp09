#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
	if (this != &other)
	{
		_bitcoinPrices = other._bitcoinPrices;
	}
	return *this;
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) {
	*this = other;
}

BitcoinExchange::~BitcoinExchange() {}

void BitcoinExchange::checkRate(float rate) {
	std::stringstream ss;
	ss << "Error: not a positive rate => " << rate;
	if (rate < 0)
		throw std::runtime_error(ss.str());
}

bool BitcoinExchange::isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int BitcoinExchange::getDaysInMonth(int year, int month) {
    switch (month) {
        case 2:
            return isLeapYear(year) ? 29 : 28;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        default:
            return 31;
    }
}

void BitcoinExchange::checkDate(int year, int month, int day, const std::string& line) {
	if (year < 0)
		throw std::runtime_error("Error: not a positive year => " + line);
	else if (month < 1 || month > 12)
		throw std::runtime_error("Error: not a valid month => " + line);
	else if (day < 1 || day > getDaysInMonth(year, month))
		throw std::runtime_error("Error: not a valid day => " + line);
}

void BitcoinExchange::checkDataCsv() {
	std::ifstream inputFile("data.csv");
	if (!inputFile.is_open())
		throw std::runtime_error("Error : csv does not open");

	std::string line;
	int year, month, day;
	char dash, comma;
    float rate;
	
    while (std::getline(inputFile, line)) {
        if (line.find("date,exchange_rate") != std::string::npos) {
            continue; // Skip header line
        }
		std::istringstream iss(line);
		
		if (!(iss >> year >> dash >> month >> dash >> day >> comma >> rate) || dash != '-' || comma != ',') {
			throw std::runtime_error("Error: invalid format => " + line);
		}

		std::stringstream ss;
		ss << std::setfill('0') << year << dash << std::setw(2) << month << dash << std::setw(2) << day;
		try
		{
			checkDate(year, month, day, ss.str());
			checkRate(rate);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
		

    }
}

// // Function to get the bitcoin price for a given date or the closest lower date
// float BitcoinExchange::getPrice(const std::string& date) const {
// 	std::map<std::string, float>::const_iterator it = bitcoinPrices.lower_bound(date);
// 	if (it != bitcoinPrices.begin()) {
// 		--it; // Move to the closest lower date
// 		return it -> second;
// 	}
// 	return -1.0; // Indicate that the date is earlier than the first entry
// }