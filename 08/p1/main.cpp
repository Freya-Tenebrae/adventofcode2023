#include <string>
#include <fstream>
#include <iostream>
#include <vector>

class Direction
{
	private:
		const std::string _position;
		const std::string _left;
		const std::string _right;

	public:
		Direction(std::string position, std::string left, std::string right);
		~Direction();

		const std::string getPosition() const;
		const std::string getLeftDirection() const;
		const std::string getRightDirection() const;
		const std::string getDestination(char direction) const;
};

Direction::Direction(std::string position, std::string left, std::string right):
	_position(position),
	_left(left),
	_right(right)
{

}

Direction::~Direction()
{

}

const std::string Direction::getPosition() const
{
	return _position;
}

const std::string Direction::getLeftDirection() const
{
	return _left;
}

const std::string Direction::getRightDirection() const
{
	return _right;
}

const std::string Direction::getDestination(char direction) const
{
	if (direction == 'L')
		return getLeftDirection();
	return getRightDirection();
}

void pars(std::string line, std::vector<Direction> &directions)
{
	if (line != "")
	{
		std::string position, left, right;

		position = line.substr(0, line.find_first_of(' ', 0));
		line.erase(0, line.find_first_of('(', 0) + 1);
		left = line.substr(0, line.find_first_of(',', 0));
		line.erase(0, line.find_first_of(' ', 0) + 1);
		right = line.substr(0, line.find_first_of(')', 0));

		directions.push_back(Direction(position, left, right));
	}
}

void resolve(std::string ways, std::vector<Direction> directions, int &total, bool debug)
{
	std::string actualPosition = "AAA";
	size_t i = 0;

	while (actualPosition != "ZZZ")
	{
		total++;
		for (std::vector<Direction>::iterator it = directions.begin(); it != directions.end(); it++)
		{
			if (actualPosition == it->getPosition())
			{
				if (debug == true)
				{
					std::cout << "Position : \033[0;34m";
					std::cout << actualPosition;
					std::cout << "\033[0m, possible destination : \033[0;34m";
					std::cout << it->getLeftDirection();
					std::cout << "\033[0m or \033[0;34m";
					std::cout << it->getRightDirection();
					std::cout << "\033[0m, direction given by the map : \033[0;34m";
					std::cout << ways[i];
					std::cout << "\033[0m, destination taken : \033[0;32m";
					std::cout << it->getDestination(ways[i]).substr();
					std::cout << "\033[0m, steps done : \033[0;33m";
					std::cout << total;
					std::cout << "\033[0m";
					std::cout << std::endl;
				}

				actualPosition = it->getDestination(ways[i]).substr();
				break;
			}
		}

		i++;
		if (i >= ways.length())
		i = 0;
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

	std::string ways = "";
	std::vector<Direction> directions;
	bool debug = false;

	if (argc == 3)
		debug = true;

	std::getline(input, line);
	ways = line.substr();

	while(std::getline(input, line))
		pars(line, directions);
		
	resolve(ways, directions, total, debug);

	std::cout << "The number of steps required to reach ZZZ is : \033[1;33m";
	std::cout << total;
	std::cout << "\033[0m" << std::endl;
	return (0);
}