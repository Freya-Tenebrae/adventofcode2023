#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>

bool isAlredyDoneForThisMotor(std::vector<std::vector<int>> &doneForThisNumber, int x, int y)
{
	std::vector<int> tmp;
	tmp.push_back(x);
	tmp.push_back(y);

	for (std::vector<std::vector<int>>::iterator it = doneForThisNumber.begin(); it != doneForThisNumber.end(); it++)
	{
		if ((*it)[0] == x && (*it)[1] == y)
			return true;
	}
	return false;
}

void resolve(std::vector<std::string> inputs, int &total, bool debug, int lineLen, int nLines)
{
	for (int i = 0; i < nLines; i++)
	{
		std::vector<std::vector<int>> NumberDoneForThisMotor;
		int totalLine = 0;

		for (int j = 0; j < lineLen; j++)
		{
			if (inputs[i][j] == '*')
			{
				int totalMotor = 1;
				int nValue = 0;
				for (int x = i - 1; x <= i + 1; x++)
				{
					for (int y = j - 1; y <= j + 1; y++)
					{
						if (x >= 0 && y >= 0 && x < nLines && y < lineLen && inputs[x][y] >= '0' && inputs[x][y] <= '9' && \
							isAlredyDoneForThisMotor(NumberDoneForThisMotor, x, y) == false)
						{
							nValue++;
							int value = 0, z = 0;

							while (y + z >= 0 && inputs[x][y + z] >= '0' && inputs[x][y + z] <= '9')
								z--;
							z++;

							while (inputs[x][y + z] >= '0' && inputs[x][y + z] <= '9')
							{
								NumberDoneForThisMotor.push_back({x, y + z});
								value = value * 10 + (inputs[x][y + z] - '0');
								z++;
							}
							totalMotor*= value;
						}
					}
				}
				if (nValue == 2)
				{
					totalLine += totalMotor;
					if (debug == true)
						{
							std::cout << "    [\033[0;34m";
							std::cout << i;
							std::cout << "\033[0m][\033[0;34m";
							std::cout << j;
							std::cout << "\033[0m], motor = \033[0;34m";
							std::cout << totalMotor;
							std::cout << "\033[0m," << std::endl;
						}
				}
			}
			else
				NumberDoneForThisMotor.clear();
		}
		total += totalLine;
		if (debug == true)
		{
			std::cout << "line \033[0;34m";
			std::cout << i;
			std::cout << "\033[0m: \033[0;30m";
			std::cout << inputs[i];
			std::cout << "\033[0m, totalLine = \033[0;32m";
			std::cout << totalLine;
			std::cout << "\033[0m," << std::endl;
		}
	}
}

void pars(std::string line, std::vector<std::string> &inputs, int &nLines, int &lineLen)
{
	inputs.push_back(line);
	lineLen = line.length();
	nLines++;
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
	std::vector<std::string> inputs;
	int total = 0, lineLen = 0, nLines = 0;
	bool debug = false;

	if (argc == 3)
		debug = true;

	while(std::getline(input, line))
		pars(line, inputs, nLines, lineLen);

	resolve(inputs, total, debug, lineLen, nLines);

	std::cout << "The sum of all of the gear ratios in your engine schematic is : \033[1;33m";
	std::cout << total;
	std::cout << "\033[0m" << std::endl;
	return (0);
}
