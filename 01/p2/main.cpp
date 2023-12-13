#include <string>
#include <fstream>
#include <iostream>
#include <vector>

std::string parse(std::string line)
{
	std::string newLine = "";

	for(size_t i = 0; i < line.length(); i++)
	{
		if (line[i] >= '0' && line[i] <= '9')
			newLine.push_back(line[i]);
		else
		{
			if (line.find("one", i) == i)
				newLine.push_back('1');
			else if (line.find("two", i) == i)
				newLine.push_back('2');
			else if (line.find("three", i) == i)
				newLine.push_back('3');
			else if (line.find("four", i) == i)
				newLine.push_back('4');
			else if (line.find("five", i) == i)
				newLine.push_back('5');
			else if (line.find("six", i) == i)
				newLine.push_back('6');
			else if (line.find("seven", i) == i)
				newLine.push_back('7');
			else if (line.find("eight", i) == i)
				newLine.push_back('8');
			else if (line.find("nine", i) == i)
				newLine.push_back('9');
		}
	}

	return newLine;
}

void resolve(std::string line, int &total, bool debug = false)
{
	char firstDigit = 0, secondDigit = 0;
	std::string newLine = parse(line);

	for(std::string::iterator it = newLine.begin(); it != newLine.end(); it++)
	{
		if (*it >= '0' && *it <= '9')
		{
			if (firstDigit == 0)
				firstDigit = *it;
			secondDigit = *it;
		}
	}

	total += (firstDigit - '0') * 10 + (secondDigit - '0');


	if (debug == true)
	{
		std::cout << "line : \033[0;30m";
		std::cout << line;
		std::cout << "\033[0m, parsed : \033[0;34m";
		std::cout << newLine;
		std::cout << "\033[0m, calibration = \033[0;32m";
		std::cout << (firstDigit - '0') * 10 + (secondDigit - '0');
		std::cout << "\033[0m," << std::endl;
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

	std::cout << "The sum of all the calibration value are : \033[1;33m";
	std::cout << total;
	std::cout << "\033[0m" << std::endl;
	return (0);
}