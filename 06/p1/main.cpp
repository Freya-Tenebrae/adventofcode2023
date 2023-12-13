#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <climits>

void pars(std::string line, std::vector<int> &values)
{
	line.erase(0, line.find(":") + 1);
	for(size_t i = 0; i < line.length(); i++)
	{
		if (line[i] != ' ')
		{
			size_t pos = line.find(" ", i);
			if (pos == std::string::npos)
				pos = line.length();
			values.push_back(std::stol(line.substr(i, pos - i)));

			while(line[i] && line[i] >= '0' && line[i] <= '9')
				i++;
		}
	}
}

void resolve(int &total, bool debug, std::vector<int> time, std::vector<int> distance)
{
	for (size_t i = 0; i < time.size() && i < distance.size(); i++)
	{
		int newDistance = 0;
		int acceleration = 0;
		int totalOfPossibleWin = 0;

		if (debug == true)
		{
			std::cout << "race \033[0;34m";
			std::cout << i + 1;
			std::cout << "\033[0m, time race : \033[0;34m";
			std::cout << time[i];
			std::cout << "ms\033[0m, distance race : \033[0;34m";
			std::cout << distance[i];
			std::cout << "mm\033[0m, relation [time acceleration / distance done] :";
			std::cout << std::endl;
		}

		for (acceleration = 0; acceleration <= time[i]; acceleration++)
		{
			newDistance = (time[i] - acceleration) * acceleration;

			if (debug == true)
			{
				std::cout << "    [";

				if (newDistance > distance[i])
					std::cout << "\033[0;34m";
				else
					std::cout << "\033[0;30m";

				std::cout << acceleration;
				std::cout << "ms \033[0m/";

				if (newDistance > distance[i])
					std::cout << "\033[0;34m ";
				else
					std::cout << "\033[0;30m ";

				std::cout << newDistance;
				std::cout << "mm\033[0m]";

				if (newDistance > distance[i])
					std::cout << ", \033[0;32mcan beat the record\033[0m";

				std::cout << std::endl;
			}

			if (newDistance > distance[i])
				totalOfPossibleWin++;
		}

		if (debug == true)
		{
			std::cout << "Number of possible way you could win = \033[0;32m";
			std::cout << totalOfPossibleWin;
			std::cout << "\033[0m";
			std::cout << std::endl;
		}

		total *= totalOfPossibleWin;
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
	int total = 1;
	bool debug = false;

	std::vector<int> time, distance;

	if (argc == 3)
		debug = true;

	std::getline(input, line);
	pars(line, time);

	std::getline(input, line);
	pars(line, distance);

	resolve(total, debug, time, distance);

	std::cout << "the number of possible way you could win for each race multiply together is : \033[1;33m";
	std::cout << total;
	std::cout << "\033[0m" << std::endl;
	return (0);
}