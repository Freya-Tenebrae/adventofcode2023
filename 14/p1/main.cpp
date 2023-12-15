#include <string>
#include <fstream>
#include <iostream>
#include <vector>

class Caneva
{
	private:
		int								_sizeX;
		int								_sizeY;
		int 							_totalLoad;
		std::vector<std::vector<char>>	_map;
		std::vector<std::vector<char>>	_mapAfterTilt;

	public:
		Caneva();
		~Caneva();

		void addLine(std::string line);
		void tiltingN();
		void computeTotalLoad();
		void print() const;

		int								getTotalLoad() const;
};

Caneva::Caneva(): _sizeX(0), _sizeY(0), _totalLoad(0)
{

}

Caneva::~Caneva()
{

}

void Caneva::addLine(std::string line)
{
	std::vector<char> tmp;

	if (_sizeX == 0)
		_sizeX = line.size();

	for (size_t i = 0; i < line.size(); i++)
		tmp.push_back(line[i]);
	_map.push_back(tmp);
	_mapAfterTilt.push_back(tmp);
	_sizeY++;
}

void Caneva::tiltingN()
{
	for (size_t i = 0; i < _mapAfterTilt.size() - 1; i++)
	{
		for (size_t j = 0; j < _mapAfterTilt[i].size(); j++)
		{
			if (_mapAfterTilt[i][j] == '.')
			{
				for (size_t k = i + 1; k < _mapAfterTilt.size(); k++)
				{
					if (_mapAfterTilt[k][j] == 'O' || _mapAfterTilt[k][j] == '#')
					{
						if (_mapAfterTilt[k][j] == 'O')
						{
							_mapAfterTilt[i][j] = 'O';
							_mapAfterTilt[k][j] = '.';
						}
						break;
					}
				}
			}
		}
	}
}

void Caneva::computeTotalLoad()
{
	for (int i = 0; i < _sizeY; i++)
	{
		for (int j = 0; j < _sizeX; j++)
		{
			if (_mapAfterTilt[i][j] == 'O')
				_totalLoad += _sizeY - i;
		}
	}
}

void Caneva::print() const
{
	for (int i = 0; i < _sizeY; i++)
	{
		for (int j = 0; j < _sizeX; j++)
		{
			if (_map[i][j] == '.')
				std::cout << "\033[0;30m.";
			else if (_map[i][j] == 'O')
				std::cout << "\033[0;34mO";
			else if (_map[i][j] == '#')
				std::cout << "\033[0;35m#";
		}

		if (i == _sizeY / 2)
			std::cout << "  \033[0;33m-->\033[0m  ";
		else
			std::cout << "       ";

		for (int j = 0; j < _sizeX; j++)
		{
			if (_mapAfterTilt[i][j] == '.' && _map[i][j] == 'O')
				std::cout << "\033[0;32m.";
			else if (_mapAfterTilt[i][j] == '.')
				std::cout << "\033[0;30m.";
			else if (_mapAfterTilt[i][j] == 'O' && _map[i][j] == '.')
				std::cout << "\033[0;32mO";
			else if (_mapAfterTilt[i][j] == 'O')
				std::cout << "\033[0;34mO";
			else if (_mapAfterTilt[i][j] == '#')
				std::cout << "\033[0;35m#";
		}

		std::cout << std::endl;
	}
	std::cout << "\033[0m";
	std::cout << std::endl;
}

int	Caneva::getTotalLoad() const
{
	return (_totalLoad);
}

void resolve(Caneva &caneva, int &total, bool debug)
{
	caneva.tiltingN();
	caneva.computeTotalLoad();

	if (debug)
	{
		std::cout << "Canevas :" << std::endl;
		caneva.print();
	}
	total = caneva.getTotalLoad();
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
	Caneva caneva;

	if (argc == 3)
		debug = true;

	while(std::getline(input, line))
		caneva.addLine(line);

	resolve(caneva, total, debug);

	std::cout << "The total load on North side is : \033[1;33m";
	std::cout << total;
	std::cout << "\033[0m" << std::endl;
	return (0);
}