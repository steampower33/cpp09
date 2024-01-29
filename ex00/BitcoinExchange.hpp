#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <stdexcept>

class BitcoinExchange {
private:
	std::map<std::string, float> _bitcoinPrices;
	BitcoinExchange(const BitcoinExchange& other);
	BitcoinExchange& operator=(const BitcoinExchange& other);
public:
	BitcoinExchange();
	~BitcoinExchange();
	void checkDataCsv();
	void checkDate(int year, int month, int day, const std::string& line);
	void checkRate(float rate);
	bool isLeapYear(int year);
	int getDaysInMonth(int year, int month);
};

#endif