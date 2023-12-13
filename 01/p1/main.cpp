#include <string>
#include <fstream>
#include <iostream>
#include <vector>

void resolve(std::string line, int &total, bool debug)
{
	char firstDigit = 0, secondDigit = 0;

	for(std::string::iterator it = line.begin(); it != line.end(); it++)
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

	std::cout << "The sum of all the calibration value are : ";
	std::cout << "\033[1;33m" << total << "\033[0m" << std::endl;
	return (0);
}