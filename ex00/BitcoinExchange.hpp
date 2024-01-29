#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>

class BitcoinExchange {
private:
	std::map<std::string, float> _bitcoinPrices;
	BitcoinExchange(const BitcoinExchange& other);
	BitcoinExchange& operator=(const BitcoinExchange& other);
public:
	BitcoinExchange();
	~BitcoinExchange();
	void checkDate(int year, int month, int day, const std::string& line);
	void checkRate(float rate);
	bool isLeapYear(int year);
	int getDaysInMonth(int year, int month);
	void checkDataCsv();
	void checkValue(float value);
	void printBitcoinPrice(std::string& date, float bitcoinPrice);
	void calcBitcoinPrice(std::string& date, float value);
	void checkInputFile(const std::string& file);
};

#endif