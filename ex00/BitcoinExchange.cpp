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
	if (rate < 0)
	{
		std::stringstream ss;
		ss << "Error: not a positive rate => " << rate;
		throw std::runtime_error(ss.str());
	}
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
	
	std::getline(inputFile, line);
    if (line.find("date,exchange_rate") == std::string::npos)
			throw std::runtime_error("Error: not a valid header in data.csv");

    while (std::getline(inputFile, line)) {
		std::istringstream iss(line);
		if (!(iss >> year >> dash >> month >> dash >> day >> comma >> rate) || dash != '-' || comma != ',') {
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
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
			continue;
		}

		_bitcoinPrices[ss.str()] = rate;
    }
	if (_bitcoinPrices.size() <= 0)
		throw std::runtime_error("Error: Empty DB");
}


void BitcoinExchange::checkValue(float value) {
	std::stringstream ss;

	if (value < 0)
	{
		ss << "Error: not a positive value => " << value;
		throw std::runtime_error(ss.str());
	}
	if (value > 1000)
	{
		ss << "Error: too large a number => " << value;
		throw std::runtime_error(ss.str());
	}
}

void BitcoinExchange::printBitcoinPrice(std::string& date, float bitcoinPrice) {
	if (bitcoinPrice >= 0) {
		std::cout << date << " => " << bitcoinPrice << " = " << bitcoinPrice << std::endl;
    } else {
		std::cerr << "Error: date not found => " << date << std::endl;
    }
}

void BitcoinExchange::calcBitcoinPrice(std::string& date, float value) {
	std::map<std::string, float>::const_iterator it = _bitcoinPrices.find(date);
	float bitcoinPrice = 0.0;

	if (it != _bitcoinPrices.end()) bitcoinPrice = it->second * value;
	else {
		it = _bitcoinPrices.lower_bound(date);
		if (it != _bitcoinPrices.begin()) {
			--it;
			bitcoinPrice = it->second * value;
		}
		else {
			std::cout <<  "Error : invalid date" << std::endl;
		}
	}
	printBitcoinPrice(date, bitcoinPrice);
}

void BitcoinExchange::checkInputFile(const std::string& file) {
	std::ifstream inputFile(file);
	if (!inputFile.is_open())
		throw std::runtime_error("Error : csv does not open");

	std::string line, date;
	int year, month, day;
	char dash, pipe;
    float value;
	
	std::getline(inputFile, line);
    if (line.find("date | value") == std::string::npos)
			throw std::runtime_error("Error: not a valid header in data.csv");

    while (std::getline(inputFile, line)) {
		std::istringstream iss(line);
		if (!(iss >> year >> dash >> month >> dash >> day >> pipe >> value) || dash != '-' || pipe != '|') {
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}

		std::stringstream ss;
		ss << std::setfill('0') << year << dash << std::setw(2) << month << dash << std::setw(2) << day;
		date = ss.str();
		try
		{
			checkDate(year, month, day, date);
			checkValue(value);
			calcBitcoinPrice(date, value);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			continue;
		}
    }
}