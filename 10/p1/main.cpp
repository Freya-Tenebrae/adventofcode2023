#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <unistd.h>

class Map
{
	private:
		int								_width;
		int								_height;
		std::vector<std::vector<char>> 	_map;
		std::vector<std::vector<int>>	_mapValue;

	public:
		Map();
		~Map();
		
		void addLine(std::string line);
		void getRightStrat();
		int computeMap(bool debug);

		void printMap(int distance) const;

};

Map::Map()
{
	_width = 0;
	_height = 0;
}

Map::~Map()
{

}
		
void Map::addLine(std::string line)
{
	std::vector<char> tmp;
	std::vector<int> tmpValue;

	if (_width == 0)
		_width = line.size();
	_height++;

	for (size_t i = 0; i < line.size(); i++)
	{
		tmp.push_back(line[i]);
		if (line[i] == 'S')
			tmpValue.push_back(0);
		else if (line[i] != '.')
			tmpValue.push_back(-2);
		else
			tmpValue.push_back(-1);
	}

	_map.push_back(tmp);
	_mapValue.push_back(tmpValue);
}

static void getDirectionToCheck(char c, bool &checkUp, bool &checkLeft, bool &checkRight, bool &checkDown)
{
	if (c == '|')
	{
		checkUp = true;
		checkDown = true;
	}
	else if (c == '-')
	{
		checkLeft = true;
		checkRight = true;
	}
	else if (c == 'L')
	{
		checkUp = true;
		checkRight = true;
	}
	else if (c == 'J')
	{
		checkUp = true;
		checkLeft = true;
	}
	else if (c == '7')
	{
		checkLeft = true;
		checkDown = true;
	}
	else if (c == 'F')
	{
		checkRight = true;
		checkDown = true;
	}
}

void Map::getRightStrat()
{
	int x, y;

	bool canGoUp = false;
	bool canGoLeft = false;
	bool canGoRight = false;
	bool canGoDown = false;

	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			if (_mapValue[i][j] == 0)
			{
				x = i;
				y = j;
			}
		}
	}

	if (x > 0 && _mapValue[x - 1][y] == -2 && (_map[x - 1][y] == '|' || _map[x - 1][y] == 'F' || _map[x - 1][y] == '7'))
		canGoUp = true;

	if (y > 0 && _mapValue[x][y - 1] == -2 && (_map[x][y - 1] == '-' || _map[x][y - 1] == 'L' || _map[x][y - 1] == 'F'))
		canGoLeft = true;

	if (y < _width - 1 && _mapValue[x][y + 1] == -2 && (_map[x][y + 1] == '-' || _map[x][y + 1] == 'J' || _map[x][y + 1] == '7'))
		canGoRight = true;

	if (x < _height - 1 && _mapValue[x + 1][y] == -2 && (_map[x + 1][y] == '|' || _map[x + 1][y] == 'L' || _map[x + 1][y] == 'J'))
		canGoDown = true;

	if (canGoUp == true && canGoDown == true)
		_map[x][y] = '|';
	else if (canGoLeft == true && canGoRight == true)
		_map[x][y] = '-';
	else if (canGoUp == true && canGoRight == true)
		_map[x][y] = 'L';
	else if (canGoUp == true && canGoLeft == true)
		_map[x][y] = 'J';
	else if (canGoLeft == true && canGoDown == true)
		_map[x][y] = '7';
	else if (canGoRight == true && canGoDown == true)
		_map[x][y] = 'F';
	else if (canGoUp == true || canGoDown == true)
		_map[x][y] = '|';
	else if (canGoLeft == true || canGoRight == true)
		_map[x][y] = '-';
	else
	{
		std::cerr << "\033[0;31mError: no way to go\033[0m" << std::endl;
		exit(-1);
	}
}

int Map::computeMap(bool debug)
{
	std::vector<std::vector<int>> positions;
	int distance = 0;
	bool isOver = false;

	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			if (_mapValue[i][j] == 0)
			{
				positions.push_back({i, j});
			}
		}
	}

	if (debug == true)
		printMap(distance);

	while (isOver == false)
	{
		std::vector<std::vector<int>> tmpPositions;
		distance++;

		for (std::vector<std::vector<int>>::iterator it = positions.begin(); it != positions.end(); ++it)
		{
			char c = _map[(*it)[0]][(*it)[1]];
			bool checkUp = false;
			bool checkLeft = false;
			bool checkRight = false;
			bool checkDown = false;
			getDirectionToCheck(c, checkUp, checkLeft, checkRight, checkDown);

			if (checkUp == true && (*it)[0] > 0 && _mapValue[(*it)[0] - 1][(*it)[1]] == -2)
			{
				_mapValue[(*it)[0] - 1][(*it)[1]] = distance;
				tmpPositions.push_back({(*it)[0] - 1, (*it)[1]});
			}

			if (checkLeft == true && (*it)[1] > 0 && _mapValue[(*it)[0]][(*it)[1] - 1] == -2)
			{
				_mapValue[(*it)[0]][(*it)[1] - 1] = distance;
				tmpPositions.push_back({(*it)[0], (*it)[1] - 1});
			}

			if (checkRight == true && (*it)[1] < _width - 1 && _mapValue[(*it)[0]][(*it)[1] + 1] == -2)
			{
				_mapValue[(*it)[0]][(*it)[1] + 1] = distance;
				tmpPositions.push_back({(*it)[0], (*it)[1] + 1});
			}

			if (checkDown == true && (*it)[0] < _height - 1 && _mapValue[(*it)[0] + 1][(*it)[1]] == -2)
			{
				_mapValue[(*it)[0] + 1][(*it)[1]] = distance;
				tmpPositions.push_back({(*it)[0] + 1, (*it)[1]});
			}
		}

		positions.clear();
		positions = tmpPositions;

		if (positions.size() == 0)
			isOver = true;

		if (debug == true)
			printMap(distance);
		
		// sleep(1);
	}

	return distance -1;
}

void Map::printMap(int distance) const
{
	std::cout << "Map at distance = \033[0;34m" << distance << "\033[0m :" << std::endl;
	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			if (_mapValue[i][j] == -2)
				std::cout << "\033[0;30m" << _map[i][j];
			else if (_mapValue[i][j] == -1)
				std::cout << "\033[0m" << " ";
			else if (_mapValue[i][j] == distance)
				std::cout << "\033[0;32m" << _map[i][j];
			else
				std::cout << "\033[0;34m" << _map[i][j];
			std::cout << "\033[0m";
		}
		std::cout << std::endl;
	}
}

void resolve(Map &map, int &total, bool debug)
{
	map.getRightStrat();
	total = map.computeMap(debug);
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
	Map map;

	if (argc == 3)
		debug = true;

	while(std::getline(input, line))
		map.addLine(line);

	resolve(map, total, debug);

	map.printMap(total);

	std::cout << "The number of steps along the loop it take to get from the starting position to the point farthest from the starting position is : \033[1;33m";
	std::cout << total;
	std::cout << "\033[0m" << std::endl;
	return (0);
}
