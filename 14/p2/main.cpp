#include <string>
#include <fstream>
#include <iostream>
#include <vector>

class Dish
{
	private:
		int								_sizeX;
		int								_sizeY;
		int 							_totalLoad;
		std::vector<std::vector<char>>	_map;
		std::vector<std::vector<std::vector<char>>>	_previousMaps;

		void tiltingN();
		void tiltingW();
		void tiltingS();
		void tiltingE();

	public:
		Dish();
		~Dish();

		void addLine(std::string line);
		std::vector<int> getCycle();
		void spinning();
		void computeTotalLoad();

		void print() const;
		int	getTotalLoad() const;
};

Dish::Dish(): _sizeX(0), _sizeY(0), _totalLoad(0)
{

}

Dish::~Dish()
{

}

void Dish::tiltingN()
{
	for (int i = 0; i < _sizeY - 1; i++)
	{
		for (int j = 0; j < _sizeX; j++)
		{
			if (_map[i][j] == '.')
			{
				for (int k = i + 1; k < _sizeY; k++)
				{
					if (_map[k][j] == 'O' || _map[k][j] == '#')
					{
						if (_map[k][j] == 'O')
						{
							_map[i][j] = 'O';
							_map[k][j] = '.';
						}
						break;
					}
				}
			}
		}
	}
}

void Dish::tiltingW()
{
	for (int j = 0; j < _sizeX - 1; j++)
	{
		for (int i = 0; i < _sizeY; i++)
		{
			if (_map[i][j] == '.')
			{
				for (int k = j + 1; k < _sizeX; k++)
				{
					if (_map[i][k] == 'O' || _map[i][k] == '#')
					{
						if (_map[i][k] == 'O')
						{
							_map[i][j] = 'O';
							_map[i][k] = '.';
						}
						break;
					}
				}
			}
		}
	}
}

void Dish::tiltingS()
{
	for (int i = _sizeY - 1; i > 0; i--)
	{
		for (int j = 0; j < _sizeX; j++)
		{
			if (_map[i][j] == '.')
			{
				for (int k = i - 1; k >= 0; k--)
				{
					if (_map[k][j] == 'O' || _map[k][j] == '#')
					{
						if (_map[k][j] == 'O')
						{
							_map[i][j] = 'O';
							_map[k][j] = '.';
						}
						break;
					}
				}
			}
		}
	}
}

void Dish::tiltingE()
{
	for (int j = _sizeX - 1; j > 0; j--)
	{
		for (int i = 0; i < _sizeY; i++)
		{
			if (_map[i][j] == '.')
			{
				for (int k = j - 1; k >= 0; k--)
				{
					if (_map[i][k] == 'O' || _map[i][k] == '#')
					{
						if (_map[i][k] == 'O')
						{
							_map[i][j] = 'O';
							_map[i][k] = '.';
						}
						break;
					}
				}
			}
		}
	}
}

void Dish::addLine(std::string line)
{
	std::vector<char> tmp;

	if (_sizeX == 0)
		_sizeX = line.size();

	for (size_t i = 0; i < line.size(); i++)
		tmp.push_back(line[i]);
	_map.push_back(tmp);
	_sizeY++;
}

std::vector<int> Dish::getCycle()
{
	for (size_t x = 0; x < _previousMaps.size(); x++)
	{
		for (size_t y = x + 1; y < _previousMaps.size(); y++)
		{
			bool same = true;
			for (int i = 0; i < _sizeY; i++)
			{
				for (int j = 0; j < _sizeX; j++)
				{
					if (_previousMaps[x][i][j] != _previousMaps[y][i][j])
					{
						same = false;
						break;
						break;
					}
				}
			}
			if (same == true)
				return {static_cast<int>(x), static_cast<int>(y)};
		}
	}
	return {};
}

void Dish::spinning()
{
	std::vector<std::vector<char>>	previousMap;
	std::vector<char>				tmp;
	for (int i = 0; i < _sizeY; i++)
	{
		for (int j = 0; j < _sizeX; j++)
		{
			tmp.push_back(_map[i][j]);
		}
		previousMap.push_back(tmp);
		tmp.clear();
	}
	_previousMaps.push_back(previousMap);

	this->tiltingN();
	this->tiltingW();
	this->tiltingS();
	this->tiltingE();
}

void Dish::computeTotalLoad()
{
	for (int i = 0; i < _sizeY; i++)
	{
		for (int j = 0; j < _sizeX; j++)
		{
			if (_map[i][j] == 'O')
				_totalLoad += _sizeY - i;
		}
	}
}

void Dish::print() const
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
		std::cout << std::endl;
	}
	std::cout << "\033[0m";
	std::cout << std::endl;
}

int	Dish::getTotalLoad() const
{
	return (_totalLoad);
}

void resolve(Dish &dish, int &total, bool debug)
{
	if (debug)
	{
		std::cout << "Starting Canevas :" << std::endl;
		dish.print();
	}

	int i, j;
	std::vector<int> cycling;
	for (i = 0; i < 1000000000; i++)
	{
		dish.spinning();
		cycling = dish.getCycle();
		if (cycling.size() == 2)
			break;

		if (debug)
		{
			std::cout << "Canevas after \033[0;34m" << i + 1 << "\033[0m spins:" << std::endl;
			dish.print();
		}
	}

	int neededCycle = (1000000000 - cycling[0] - 1) % (cycling[1] - cycling[0]);

	for (j = 0; j < neededCycle; j++)
	{
		dish.spinning();
		if (debug)
		{
			std::cout << "Canevas after \033[0;34m" << i + j + 1 << "\033[0m spins:" << std::endl;
			dish.print();
		}
	}

	if (debug)
	{
		std::cout << "After \033[0;32m";
		std::cout <<  cycling[0];
		std::cout << "\033[0m spin cycle, the position of all round rock will cycle each \033[0;32m";
		std::cout <<  (cycling[1] - cycling[0]);
		std::cout << "\033[0m spin cycle.";
		std::cout << std::endl;
		std::cout << "Canevas after \033[0;34m1000000000\033[0m spins cycle will look like the \033[0;32m";
		std::cout << i + j + 1 - (cycling[1] - cycling[0]);
		std::cout << "th\033[0m or \033[0;32m";
		std::cout << i + j + 1;
		std::cout << "th\033[0m or \033[0;33m";
		std::cout << i + j + 1 - (cycling[1] - cycling[0]);
		std::cout << " + ";
		std::cout << (cycling[1] - cycling[0]);
		std::cout << " * n\033[0m spin cycle:";
		std::cout << std::endl;
		dish.print();
		std::cout << std::endl;
	}

	dish.computeTotalLoad();
	total = dish.getTotalLoad();
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
	Dish dish;

	if (argc == 3)
		debug = true;

	while(std::getline(input, line))
		dish.addLine(line);

	resolve(dish, total, debug);

	std::cout << "The total load on the north support beams is : \033[1;33m";
	std::cout << total;
	std::cout << "\033[0m" << std::endl;
	return (0);
}
