#include <string>
#include <fstream>
#include <iostream>
#include <vector>

void resolve(std::string line, int &total, bool debug)
{
	if (debug)
		std::cout << "Line = " << line << std::endl;
	for (std::string::iterator it = line.begin(); it != line.end(); it++)
		total++;
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

	std::cout << "The number of line is : \033[1;33m";
	std::cout << total;
	std::cout << "\033[0m" << std::endl;
	return (0);
}