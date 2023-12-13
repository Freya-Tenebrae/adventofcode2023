#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

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

void getAllStratPositions(std::vector<std::string> &actualPositions, std::vector<Direction> directions)
{
	for (std::vector<Direction>::iterator it = directions.begin(); it != directions.end(); it++)
	{
		if (it->getPosition()[2] == 'A')
			actualPositions.push_back(it->getPosition().substr());
	}
}

void getAllPotentialFinalPositions(std::vector<std::string> &PotentialFinalDestination, std::vector<Direction> directions)
{
	for (std::vector<Direction>::iterator it = directions.begin(); it != directions.end(); it++)
	{
		if (it->getPosition()[2] == 'Z')
			PotentialFinalDestination.push_back(it->getPosition().substr());
	}
}

void getallFinalPositionForEachStart(std::vector<std::map<std::string, int>> &allFinalPositionForEachStart, std::vector<std::string> actualPositions, std::vector<std::string> PotentialFinalDestination)
{
	for (std::vector<std::string>::iterator it = actualPositions.begin(); it != actualPositions.end(); it++)
	{
		std::map<std::string, int> finalPositionForEachStart;
		for (std::vector<std::string>::iterator it2 = PotentialFinalDestination.begin(); it2 != PotentialFinalDestination.end(); it2++)
			finalPositionForEachStart.insert(std::pair<std::string, int>(*it2, 0));
		allFinalPositionForEachStart.push_back(finalPositionForEachStart);
	}

}

int gcd(int a, int b) 
{ 
  if (b == 0) 
    return a; 
  return gcd(b, a % b); 
} 
  
long long lcm(std::vector<int> v) 
{
	if (v.size() == 1)
		return v[0];

	std::vector<int> vb;
	for (size_t i = 0; i < v.size() - 1; i++)
		vb.push_back(v[i]);

	long long totalGcd;

	for (size_t i = 0; i < v.size() - 1; i++)
		totalGcd = gcd(v[i], v[i + 1]);

    return lcm(vb) * v[v.size() - 1] / totalGcd; 
}

void computeTotal(std::vector<std::string> stratPosition, std::vector<std::map<std::string, int>> allFinalPositionForEachStart, long long &total, bool debug)
{
	std::vector<int> AllStepsFromStratToEnd;
	for (size_t x = 0; x < stratPosition.size(); x++)
	{
		if (debug == true)
		{
			std::cout << "Start Position : \033[0;34m";
			std::cout << stratPosition[x];
			std::cout << " can end on : \033[0;34m";
		}
		for (std::map<std::string, int>::iterator it = allFinalPositionForEachStart[x].begin(); it != allFinalPositionForEachStart[x].end(); it++)
		{
			if (it->second != 0)
				AllStepsFromStratToEnd.push_back(it->second);
			if (debug == true)
			{
				std::cout << it->first;
				std::cout << "\033[0m with \033[0;34m";
				if (it->second == 0)
					std::cout << "\033[0;35m-";
				else
					std::cout << it->second;
				std::cout << "\033[0m steps, \033[0;34m";
			}
		}
		if (debug == true)
		{
			std::cout << "\033[0m";
			std::cout << std::endl;
		}
	}
	total = lcm(AllStepsFromStratToEnd);
}


void resolve(std::string ways, std::vector<Direction> directions, long long &total, bool debug)
{
	std::vector<std::string> actualPositions, stratPosition;
	std::vector<std::string> PotentialFinalDestination;
	std::vector<std::map<std::string, int>> allFinalPositionForEachStart;

	getAllStratPositions(stratPosition, directions);
	getAllStratPositions(actualPositions, directions);
	getAllPotentialFinalPositions(PotentialFinalDestination, directions);
	getallFinalPositionForEachStart(allFinalPositionForEachStart, actualPositions, PotentialFinalDestination);


	size_t i = 0;
	for (size_t nSteps = 1; nSteps <= ways.size() * directions.size(); nSteps++)
	{
		for (size_t x = 0; x < actualPositions.size(); x++)
		{
			for (std::vector<Direction>::iterator it = directions.begin(); it != directions.end(); it++)
			{
				if (actualPositions[x] == it->getPosition())
				{
					if (debug == true)
					{
						std::cout << "Position : \033[0;34m";
						std::cout << actualPositions[x];
						std::cout << "\033[0m, possible destination : \033[0;34m";
						std::cout << it->getLeftDirection();
						std::cout << "\033[0m or \033[0;34m";
						std::cout << it->getRightDirection();
						std::cout << "\033[0m, direction given by the map : \033[0;34m";
						std::cout << ways[i];
						std::cout << "\033[0m, destination taken : \033[0;32m";
						std::cout << it->getDestination(ways[i]).substr();
						std::cout << "\033[0m, steps done : \033[0;33m";
						std::cout << nSteps;
						std::cout << "\033[0m";
						std::cout << std::endl;
					}
					actualPositions[x] = it->getDestination(ways[i]).substr();
					if (actualPositions[x][2] == 'Z')
					{
						for (std::map<std::string, int>::iterator it2 = allFinalPositionForEachStart[x].begin(); it2 != allFinalPositionForEachStart[x].end(); it2++)
						{
							if (it2->first == actualPositions[x] && it2->second == 0)
							{
								it2->second = nSteps;
								if (debug == true)
								{
									std::cout << "Start Position : \033[0;34m";;
									std::cout << stratPosition[x]; 
									std::cout << " can end on : \033[0;34m";
									std::cout << it2->first;
									std::cout << "\033[0m with \033[0;34m";
									std::cout << nSteps;
									std::cout << "\033[0m steps.\033[0m";
									std::cout << std::endl;
								}
							}
						}
					}
					break;
				}
			}
		}
		i++;
		if (i >= ways.length())
			i = 0;
	}
	computeTotal(stratPosition, allFinalPositionForEachStart, total, debug);
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
	long long total = 1;

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