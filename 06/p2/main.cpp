#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <climits>

void pars(std::string line, long &values)
{
	line.erase(0, line.find(":") + 1);

	std::string valueString = "";
	for(size_t i = 0; i < line.length(); i++)
	{
		if (line[i] != ' ')
		{
			size_t pos = line.find(" ", i);
			if (pos == std::string::npos)
				pos = line.length();

			valueString.append(line.substr(i, pos - i));

			while(line[i] && line[i] >= '0' && line[i] <= '9')
				i++;
		}
	}

	values = std::stol(valueString);
}

void resolve(long &total, bool debug, long time, long distance)
{
	long newDistance = 0;
	long acceleration = 0;

	for (acceleration = 0; acceleration <= time; acceleration++)
	{
		newDistance = (time - acceleration) * acceleration;

		if (debug == true)
		{
			std::cout << "time race : \033[0;34m";
			std::cout << time;
			std::cout << "ms\033[0m, distance race : \033[0;34m";
			std::cout << distance;
			std::cout << "mm\033[0m, time acceleration :";

			if (newDistance > distance)
				std::cout << "\033[0;34m";
			else
				std::cout << "\033[0;30m";
			
			std::cout << acceleration;
			std::cout << "ms\033[0m, distance done :";

			if (newDistance > distance)
				std::cout << "\033[0;34m ";
			else
				std::cout << "\033[0;30m ";
			
			std::cout << newDistance;
			std::cout << "mm\033[0m";
			if (newDistance > distance)
				std::cout << ", \033[0;32mcan beat the record\033[0m";
			std::cout << std::endl;
		}

		if (newDistance > distance)
			total++;
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
	long total = 0;
	bool debug = false;

	long time, distance;

	if (argc == 3)
		debug = true;

	std::getline(input, line);
	pars(line, time);

	std::getline(input, line);
	pars(line, distance);

	resolve(total, debug, time, distance);

	std::cout << "The total of all races number of way to beat the recor multiply beetween them : \033[1;33m";
	std::cout << total;
	std::cout << "\033[0m" << std::endl;
	return (0);
}