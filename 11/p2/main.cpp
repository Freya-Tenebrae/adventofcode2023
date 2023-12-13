#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

#define EXPANDFACTOR 1000000

class Galaxy
{
	private:
		const int	_id;
		int			_x;
		int			_y;

	public:
		Galaxy(int id, int x, int y);
		~Galaxy();

		int getId() const;
		int getX() const;
		int getY() const;
		int getShortestDistanceBeetweenTwoGalaxy(const Galaxy &other) const;

		void addExpandCorrection(int x, int y);
}; //!Galaxy

Galaxy::Galaxy(int id, int x, int y):
	_id(id),
	_x(x),
	_y(y)
{

}

Galaxy::~Galaxy()
{

}

int Galaxy::getId() const
{
	return (this->_id);
}

int Galaxy::getX() const
{
	return (this->_x);
}

int Galaxy::getY() const
{
	return (this->_y);
}

int Galaxy::getShortestDistanceBeetweenTwoGalaxy(const Galaxy &other) const
{
	return (std::abs(this->_x - other.getX()) + std::abs(this->_y - other.getY()));
}


void Galaxy::addExpandCorrection(int x, int y)
{
	this->_x += x;
	this->_y += y;
}

class Universe
{
	private:
		std::vector<Galaxy>	_galaxies;
		int					_sizeX;
		int					_sizeY;

	public:
		Universe();
		~Universe();

		void addGalaxy(int x, int y);
		void expand();

		std::vector<long long> getAllDistancesBeetweenGalaxy(bool debug) const;

		void showAllGalaxy() const;

}; //!Universe

Universe::Universe()
{
	_sizeX = 0;
	_sizeY = 0;
}

Universe::~Universe()
{

}

void Universe::addGalaxy(int x, int y)
{
	if (_sizeX < x)
		_sizeX = x;

	if (_sizeY < y)
		_sizeY = y;

	this->_galaxies.push_back(Galaxy(this->_galaxies.size() + 1, x, y));
}

void Universe::expand()
{
	std::vector<int> row_to_expand, line_to_expand;
	for (int i = 0; i < this->_sizeX; ++i)
		row_to_expand.push_back(i);

	for (int i = 0; i < this->_sizeY; ++i)
		line_to_expand.push_back(i);

	for (std::vector<Galaxy>::iterator it = this->_galaxies.begin(); it != this->_galaxies.end(); ++it)
	{
		std::vector<int>::iterator itX = std::find(row_to_expand.begin(), row_to_expand.end(), it->getX());
		if (itX != row_to_expand.end())
			row_to_expand.erase(itX);

		std::vector<int>::iterator itY = std::find(line_to_expand.begin(), line_to_expand.end(), it->getY());
		if (itY != line_to_expand.end())
			line_to_expand.erase(itY);
	}

	this->_sizeX += row_to_expand.size() * (EXPANDFACTOR - 1);
	this->_sizeY += line_to_expand.size() * (EXPANDFACTOR - 1);

	for (std::vector<Galaxy>::iterator it = this->_galaxies.begin(); it != this->_galaxies.end(); ++it)
	{
		int expendCorrectionX = 0, expendCorrectionY = 0;

		for (std::vector<int>::iterator itX = row_to_expand.begin(); itX != row_to_expand.end(); ++itX)
		{
			if (it->getX() > *itX)
				expendCorrectionX += (EXPANDFACTOR - 1);
			else
				break;
		}

		for (std::vector<int>::iterator itY = line_to_expand.begin(); itY != line_to_expand.end(); ++itY)
		{
			if (it->getY() > *itY)
				expendCorrectionY += (EXPANDFACTOR - 1);
			else
				break;
		}

		it->addExpandCorrection(expendCorrectionX, expendCorrectionY);
	}
}

std::vector<long long> Universe::getAllDistancesBeetweenGalaxy(bool debug) const
{
	std::vector<long long> distances;
	for (std::vector<Galaxy>::const_iterator it = this->_galaxies.begin(); it != this->_galaxies.end(); ++it)
	{
		for (std::vector<Galaxy>::const_iterator it2 = it + 1; it2 != this->_galaxies.end(); ++it2)
		{
			int distance = it->getShortestDistanceBeetweenTwoGalaxy(*it2);
			distances.push_back(distance);

			if (debug == true)
			{
				std::cout << "Distance beetween galaxy \033[0;34m";
				std::cout << it->getId();
				std::cout << "\033[0m to galaxy \033[0;34m";
				std::cout << it2->getId();
				std::cout << "\033[0m is \033[0;32m";
				std::cout << distance;
				std::cout << "\033[0m" << std::endl;
			}
		}
	}
	return (distances);

}

void Universe::showAllGalaxy() const
{
	for (std::vector<Galaxy>::const_iterator it = this->_galaxies.begin(); it != this->_galaxies.end(); ++it)
	{
		std::cout << "Galaxy \033[0;34m";
		std::cout << it->getId();
		std::cout << "\033[0m : [\033[0;34m";
		std::cout << it->getX();
		std::cout << "\033[0m/\033[0;34m";
		std::cout << it->getY();
		std::cout << "\033[0m]";
		std::cout << std::endl;
	}
}

void pars(std::string line, Universe &universe, int &i)
{
	for (std::string::iterator it = line.begin(); it != line.end(); ++it)
	{
		if (*it == '#')
			universe.addGalaxy(i, it - line.begin());
	}
	i++;
}

void resolve(Universe &universe, long long &total, int debug)
{
	universe.expand();

	if (debug == true)
		universe.showAllGalaxy();

	std::vector<long long> distances = universe.getAllDistancesBeetweenGalaxy(debug);
	for (std::vector<long long>::iterator it = distances.begin(); it != distances.end(); ++it)
		total += *it;
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
	long long total = 0;
	int i = 0;
	Universe universe;

	bool debug = false;

	if (argc == 3)
		debug = true;

	while(std::getline(input, line))
		pars(line, universe, i);

	resolve(universe, total, debug);

	std::cout << "the sum of the distance beetween each pair of galaxy is when expension is \033[0;34m";
	std::cout << EXPANDFACTOR;
	std::cout << "\033[0m is : \033[1;33m";
	std::cout << total;
	std::cout << "\033[0m" << std::endl;
	return (0);
}