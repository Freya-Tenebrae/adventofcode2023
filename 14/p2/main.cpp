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
		
		std::vector<std::vector<std::vector<char>>>	_previousMaps;

	public:
		Caneva();
		~Caneva();

		void addLine(std::string line);
		void tiltingN();
		void tiltingW();
		void tiltingS();
		void tiltingE();
		std::vector<int> getCycle();
		void spinning();
		void computeTotalLoad();
		void print() const;
		void spinDidntChange();

		int								getSizeX() const;
		int								getSizeY() const;
		int								getTotalLoad() const;
		std::vector<std::vector<char>>	getMap() const;
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
	_sizeY++;
}

void Caneva::tiltingN()
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

void Caneva::tiltingW()
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

void Caneva::tiltingS()
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

void Caneva::tiltingE()
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

std::vector<int> Caneva::getCycle()
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

void Caneva::spinning()
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

void Caneva::computeTotalLoad()
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

void Caneva::print() const
{
	for (int i = 0; i < _sizeY; i++)
	{
		for (int j = 0; j < _sizeX; j++)
			std::cout << _map[i][j];
		std::cout << std::endl;
	}
}

int	Caneva::getSizeX() const
{
	return (_sizeX);
}

int	Caneva::getSizeY() const
{
	return (_sizeY);
}

int	Caneva::getTotalLoad() const
{
	return (_totalLoad);
}

std::vector<std::vector<char>>	Caneva::getMap() const
{
	return (_map);
}








void resolve(Caneva &caneva, int &total, bool debug)
{
	if (debug)
	{
		std::cout << "Starting Canevas :" << std::endl;
		caneva.print();
		std::cout << std::endl;
		std::cout << std::endl;
	}

	int i;
	std::vector<int> cycling;
	for (i = 0; i < 1000000000; i++)
	{
		caneva.spinning();
		cycling = caneva.getCycle();
		if (cycling.size() == 2)
			break;

		if (debug)
		{
			std::cout << "Canevas after " << i + 1 << " spins:" << std::endl;
			caneva.print();
			std::cout << std::endl;
			std::cout << std::endl;
		}
	}

	int neededCycle = (1000000000 - cycling[0] - 1) % (cycling[1] - cycling[0]);

	for (int j = 0; j < neededCycle; j++)
	{
		caneva.spinning();
		if (debug)
		{
			std::cout << "Canevas after " << i + j + 1 << " spins:" << std::endl;
			caneva.print();
			std::cout << std::endl;
			std::cout << std::endl;
		}
	}

	std::cout << "first cycling at " <<  cycling[0] << std::endl;
	std::cout << "to " <<  cycling[1] << std::endl;

	caneva.computeTotalLoad();
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
