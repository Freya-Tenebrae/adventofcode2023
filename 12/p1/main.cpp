#include <string>
#include <fstream>
#include <iostream>
#include <vector>

bool isValid(std::string baseReport, std::vector<int> value)
{
	std::vector<int> actualValue;
	int currentValue = 0;

	for (size_t i = 0; i < baseReport.length(); i++)
	{
		if (baseReport[i] == '#')
			currentValue++;
		else
		{
			if (currentValue != 0)
				actualValue.push_back(currentValue);
			currentValue = 0;
		}
	}
	if (currentValue != 0)
		actualValue.push_back(currentValue);

	if (value.size() != actualValue.size())
		return false;

	for (size_t i = 0; i < value.size(); i++)
	{
		if (value[i] != actualValue[i])
			return false;
	}

	return true;

}

bool canBeValidAtThisPoint(std::string baseReport, std::vector<int> value)
{
	std::vector<int> actualValue;
	int currentValue = 0;

	for (size_t i = 0; i < baseReport.length(); i++)
	{
		if (baseReport[i] == '#')
			currentValue++;
		else if (baseReport[i] == '.')
		{
			if (currentValue != 0)
				actualValue.push_back(currentValue);
			currentValue = 0;
		}
		else
			break;
	}
	if (currentValue != 0)
		actualValue.push_back(currentValue);

	if (value.size() < actualValue.size())
		return false;

	for (size_t i = 0; i < actualValue.size(); i++)
	{
		if (value[i] < actualValue[i])
			return false;
	}

	return true;
}

void computeAllValidPotentialConditionReport(std::string baseReport, std::vector<std::string> &potentialConditionReport, std::vector<int> value)
{
	size_t posFirstUnknown = baseReport.find_first_of('?');
	if (posFirstUnknown == std::string::npos)
	{
		if (isValid(baseReport, value) == true)
			potentialConditionReport.push_back(baseReport);
	}
	else
	{
		baseReport[posFirstUnknown] = '.';
		if (canBeValidAtThisPoint(baseReport, value) == true)
			computeAllValidPotentialConditionReport(baseReport, potentialConditionReport, value);
		baseReport[posFirstUnknown] = '#';
		if (canBeValidAtThisPoint(baseReport, value) == true)
			computeAllValidPotentialConditionReport(baseReport, potentialConditionReport, value);
	}
}

void resolve(std::string line, int &total, bool debug)
{
	std::string baseReport;
	std::vector<std::string> potentialConditionReport;
	std::vector<int> value;
	int arrangements = 0;
	baseReport = line.substr(0, line.find_first_of(' '));
	line.erase(0, line.find_first_of(' ') + 1);
	while (line.size() > 0)
	{
		size_t posComa = line.find_first_of(',');
		if (posComa == std::string::npos)
			posComa = line.size();
	
		value.push_back(stoi(line.substr(0, posComa)));
		line.erase(0, posComa + 1);
	}

	computeAllValidPotentialConditionReport(baseReport, potentialConditionReport, value);

	arrangements = potentialConditionReport.size();

	total += arrangements;

	if (debug == true)
	{
		std::cout << "\033[0;34m";
		std::cout << baseReport;
		std::cout << "\033[0m have \033[0;34m";
		for (std::vector<int>::iterator it = value.begin(); it != value.end(); it++)
		{
			if (it + 1 == value.end())
				std::cout << "\033[0mand \033[0;34m";
			std::cout << *it;
			if (it + 1 != value.end())
				std::cout << "\033[0m, \033[0;34m";
		}
		std::cout << "\033[0m contiguous group of damaged springs. The \033[0;32m" << arrangements << "\033[0m possibles Arrangement are :\033[0;32m";
		std::cout << std::endl;
		for (std::vector<std::string>::iterator it = potentialConditionReport.begin(); it != potentialConditionReport.end(); it++)
		{
			std::cout << *it;
			std::cout << std::endl;
		}
		std::cout << "\033[0m";
		std::cout << std::endl;
	}
}

int main(int argc, char const *argv[])
{
	if (argc < 2 || argc > 3)
	{
		std::cerr << "\033[0;31mError: wrong numbers of arguments\033[0m" << std::endl;
		return (-1);
	}
	std::ifstream input(argv[1]);
	std::string line;
	int total = 0;
	bool debug = false;

	if (argc == 3)
		debug = true;

	while(std::getline(input, line))
		resolve(line, total, debug);

	std::cout << "The sum of all arrangements possible is : \033[1;33m";
	std::cout << total;
	std::cout << "\033[0m" << std::endl;
	return (0);
}