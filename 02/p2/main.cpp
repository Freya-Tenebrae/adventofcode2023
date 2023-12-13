#include <string>
#include <fstream>
#include <iostream>
#include <vector>

int maximumOf(std::vector<int> ColorNumberVector)
{
	int number = 0;

	for (std::vector<int>::iterator it = ColorNumberVector.begin(); it != ColorNumberVector.end(); it++)
	{
		if (*it > number)
			number = *it;
	}

	return number;
}


void getNumberOfColors(std::string part, std::vector<int> &red, std::vector<int> &green, std::vector<int> &blue)
{
	size_t posComa = part.find(','), posActual = 0, posSpace = 0;
	
	while (posActual != std::string::npos)
	{
		posSpace = part.find(' ', posActual);
		std::string value = part.substr(posActual, posSpace - posActual);
		std::string color = part.substr(posSpace + 1, posComa - posSpace - 1);
		int nColor = std::stoi(value);

		if (color == "red")
			red.push_back(nColor);
		else if (color == "green")
			green.push_back(nColor);
		else if (color == "blue")
			blue.push_back(nColor);
		else
			std::cerr << "\033[0;31mError: wrong color >" << color << "<\033[0m" << std::endl;

		if (posComa == std::string::npos)
			posActual = posComa;
		else
		{
			posActual = posComa + 2;
			posComa = part.find_first_of(',', posActual);
		}
	}
}

void resolve(std::string line, int &total, bool debug)
{
	std::string gameIdString, part;
	std::vector<int> red, green, blue;
	int gameId, maxRed, maxGreen, maxBlue;

	if (debug == true)
		std::cout << "line : \033[0;30m" << line << "\033[0m";

	gameIdString = line.substr(0, line.find_first_of(':'));
	gameIdString.erase(0, gameIdString.find_first_of(' '));
	gameId = std::stoi(gameIdString);

	line.erase(0, line.find_first_of(':') + 2);

	while (line.length() > 0)
	{
		if (line.find_first_of(';') != std::string::npos)
		{
			part = line.substr(0, line.find_first_of(';'));
			getNumberOfColors(part, red, green, blue);
			line.erase(0, line.find_first_of(';') + 2);
		}
		else
		{
			part = line;
			getNumberOfColors(part, red, green, blue);
			line.erase(0, line.length());
		}
	}

	maxRed = maximumOf(red);
	maxGreen = maximumOf(green);
	maxBlue = maximumOf(blue);

	total += maxRed * maxGreen * maxBlue;

	if (debug == true)
	{
		std::cout << "\033[0m, gameId = \033[0;34m";
		std::cout << gameId;
		std::cout << "\033[0m, minRed = \033[0;34m";
		std::cout << maxRed;
		std::cout << "\033[0m, minGreen = \033[0;34m";
		std::cout << maxGreen;
		std::cout << "\033[0m, minBlue = \033[0;34m";
		std::cout << maxBlue;
		std::cout << "\033[0m, totalToAdd = \033[0;32m";
		std::cout << maxRed * maxGreen * maxBlue;
		std::cout << "\033[0m" << std::endl;
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

	std::cout << "The sum of the power of the minimum set of cubes is : \033[1;33m";
	std::cout << total;
	std::cout << "\033[0m" << std::endl;
	return (0);
}