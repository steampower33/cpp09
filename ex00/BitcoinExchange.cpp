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
		std::ostringstream oss;
		oss << "Error: not a positive rate => " << rate;
		throw std::runtime_error(oss.str());
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
	char dash1, dash2, comma;
    float rate;
	int cnt;
	size_t prePos, pos;
	
	std::getline(inputFile, line);
    if (std::strcmp(line.c_str(), "date,exchange_rate"))
		throw std::runtime_error("Error: not a valid header in db file");

    while (std::getline(inputFile, line)) {
		prePos = 0;
		pos = 0;
		cnt = 0;
		std::istringstream iss(line);
		
		if (!(iss >> year >> dash1 >> month >> dash2 >> day >> comma >> rate) || dash1 != '-' || dash2 != '-' || comma != ',') {
			std::ostringstream errorOss;
			errorOss << "Error: not a valid data in data.csv => " << line;
			throw std::runtime_error(errorOss.str());
		}
		if (line.find(' ') != std::string::npos)
			throw std::runtime_error("Error: line has space");

		pos = 0;
		pos = line.find('-', pos);
		if (line[0] == '0') {
			std::cout << "Error: year starts from 0" << std::endl;
			continue;
		}
		prePos = pos;

		pos = line.find('-', pos + 1);
		if (pos - prePos != 3) {
			std::cout << "Error: month length is not 2" << std::endl;
			continue;
		}
		prePos = pos;

		pos = line.find(',', pos + 1);
		if (pos - prePos != 3) {
			std::cout << "Error: day length is not 2" << std::endl;
			continue;
		}
		prePos = pos;

		std::ostringstream oss;
		oss << std::setfill('0') << std::setw(4) << year << dash1 << std::setw(2) << month << dash2 << std::setw(2) << day;
		checkDate(year, month, day, oss.str());
		checkRate(rate);
		_bitcoinPrices[oss.str()] = rate;
    }
	if (_bitcoinPrices.size() <= 0)
		throw std::runtime_error("Error: Empty DB");
}


void BitcoinExchange::checkValue(float value) {
	std::ostringstream ss;

	if (value <= 0)
	{
		ss << "Error: not a positive value => " << value;
		throw std::runtime_error(ss.str());
	}
	if (value >= 1000)
	{
		ss << "Error: too large a number => " << value;
		throw std::runtime_error(ss.str());
	}
}

void BitcoinExchange::printBitcoinPrice(std::string& date, float value, float bitcoinPrice) {
	if (bitcoinPrice >= 0) {
		std::cout << date << " => " << value << " = " << bitcoinPrice << std::endl;
    } else {
		std::cout << "Error: not a positive value => " << bitcoinPrice << std::endl;
    }
}

void BitcoinExchange::calcBitcoinPrice(std::string& date, float value) {
	std::map<std::string, float>::iterator it = _bitcoinPrices.find(date);
	float bitcoinPrice = 0.0;

	if (it != _bitcoinPrices.end())
		bitcoinPrice = it->second * value;
	else {
		it = _bitcoinPrices.lower_bound(date);
		if (it != _bitcoinPrices.begin()) {
			--it;
			bitcoinPrice = it->second * value;
		}
		else {
			std::cout <<  "Error : invalid date => " << date << std::endl;
			return ;
		}
	}
	printBitcoinPrice(date, value, bitcoinPrice);
}

void BitcoinExchange::checkInputFile(const std::string& file) {
	std::ifstream inputFile(file.c_str());
	if (!inputFile.is_open())
		throw std::runtime_error("Error : csv does not open");

	std::string line, date;
	int year, month, day;
	char dash1, dash2, pipe;
    float value;
	int cnt, firstDataYearLengthInDb;
	size_t prePos, pos;
	
	std::getline(inputFile, line);
    if (std::strcmp(line.c_str(), "date | value"))
		throw std::runtime_error("Error: not a valid header in input file");

    while (std::getline(inputFile, line)) {
		prePos = 0;
		pos = 0;
		cnt = 0;
		firstDataYearLengthInDb = 0;
		std::istringstream iss(line);
		if (!(iss >> year >> dash1 >> month >> dash2 >> day >> pipe >> value) || dash1 != '-' || dash2 != '-' || pipe != '|') {
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}

		pos = line.find(' ', pos);
		if (pos != std::string::npos)
			cnt++;
		pos = line.find(' ', pos + 1);
		if (pos != std::string::npos)
			cnt++;
		pos = line.find(' ', pos + 1);
		if (pos != std::string::npos)
			cnt++;
		if (cnt != 2) {
			std::cout << "Error: line has space more than 2 => " << line << std::endl;
			continue;
		}

		pos = 0;
		pos = line.find('-', pos);
		if (line[0] == '0') {
			std::cout << "Error: year starts from 0" << std::endl;
			continue;
		}
		prePos = pos;

		pos = line.find('-', pos + 1);
		if (pos - prePos != 3) {
			std::cout << "Error: month length is not 2" << std::endl;
			continue;
		}
		prePos = pos;

		pos = line.find(' ', pos + 1);
		if (pos - prePos != 3) {
			std::cout << "Error: day length is not 2" << std::endl;
			continue;
		}
		prePos = pos;
			
		firstDataYearLengthInDb = _bitcoinPrices.begin()->first.find('-');

		std::ostringstream ss;
		ss << std::setfill('0') << std::setw(firstDataYearLengthInDb) << year << dash1 << std::setw(2) << month << dash2 << std::setw(2) << day;
		date = ss.str();
		try
		{
			checkDate(year, month, day, date);
			checkValue(value);
			calcBitcoinPrice(date, value);
		}
		catch(const std::exception& e)
		{
			std::cout << e.what() << '\n';
		}
    }
}