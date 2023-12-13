#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

class Caneva
{
	private:
		int								_sizeX;
		int								_sizeY;
		int								_smudgeX;
		int								_smudgeY;
		bool							_reflectionIsVertical;
		bool							_reflectionIsHorizontal;
		int								_valueVerticalReflection;
		int								_valueHorizontalReflection;
		int								_valueReflection;
		std::vector<std::vector<char>>	_map;

	public:
		Caneva();
		~Caneva();

		void addLine(std::string line);
		void computeSmudge();
		void computeReflection(int exception, int smudgeY, int smudgeX);
		void computeVerticalReflection(int smudgeX);
		void computeHorizontalReflection(int smudgeY);

		int								getSizeX() const;
		int								getSizeY() const;
		int								getSmudgeX() const;
		int								getSmudgeY() const;
		bool							reflectionIsVertical() const;
		bool							reflectionIsHorizontal() const;
		int								getValueVerticalReflection() const;
		int								getValueHorizontalReflection() const;
		int								getValueReflection() const;
		std::vector<std::vector<char>>	getMap() const;
};

Caneva::Caneva()
{
	_sizeX = 0;
	_sizeY = 0;
	_smudgeX = 0;
	_smudgeY = 0;
	_reflectionIsVertical = false;
	_reflectionIsHorizontal = false;
	_valueVerticalReflection = 0;
	_valueHorizontalReflection = 0;
	_valueReflection = 0;
}

Caneva::~Caneva()
{

}

void Caneva::addLine(std::string line)
{
	if (this->_sizeX == 0)
		_sizeX = static_cast<int>(line.size());
	_sizeY++;

	std::vector<char> tmp;
	for (size_t i = 0; i < line.size(); i++)
	{
		tmp.push_back(line[i]);
	}
	_map.push_back(tmp);
}

void Caneva::computeSmudge()
{
	this->computeReflection(-1, -1, -1);
	int exception = this->_valueReflection;
	this->_reflectionIsVertical = false;
	this->_reflectionIsHorizontal = false;
	this->_valueVerticalReflection = 0;
	this->_valueHorizontalReflection = 0;
	this->_valueReflection = 0;

	for (int i = 0; i < _sizeY; i++)
	{
		for (int j = 0; j < _sizeX; j++)
		{
			if (this->_map[i][j] == '#')
				this->_map[i][j] = '.';
			else
				this->_map[i][j] = '#';
			
			this->computeReflection(exception, i, j);

			if (this->_reflectionIsVertical == false && this->_reflectionIsHorizontal == false)
			{
				if (this->_map[i][j] == '#')
					this->_map[i][j] = '.';
				else
					this->_map[i][j] = '#';
			}
			else
			{
				_smudgeY = i;
				_smudgeX = j;
				return ;
			}
		}
	}
}

void Caneva::computeReflection(int exception, int smudgeY, int smudgeX)
{
	this->computeVerticalReflection(smudgeX);
	if (this->_valueReflection == exception)
	{
		this->_reflectionIsVertical = false;
		this->_valueVerticalReflection = 0;
		this->_valueReflection = 0;
	}

	if (this->_reflectionIsVertical == false)
	{
		this->computeHorizontalReflection(smudgeY);
		if (this->_valueReflection == exception)
		{
			this->_reflectionIsHorizontal = false;
			this->_valueHorizontalReflection = 0;
			this->_valueReflection = 0;
		}
	}
}

void Caneva::computeVerticalReflection(int smudgeX)
{
	std::vector<int> verticalPotentialReflections;
	for (int i = 0; i < _sizeY; i++)
	{
		for (int j = 1; j < _sizeX; j++)
		{
			if (i == 0 || std::find(verticalPotentialReflections.begin(), verticalPotentialReflections.end(), j) != verticalPotentialReflections.end())
			{
				std::vector<char> leftPart;
				std::vector<char> rightPart;

				for (int k = j - 1; k >= 0; k--)
					leftPart.push_back(_map[i][k]);
				for (int k = j; k < _sizeX; k++)
					rightPart.push_back(_map[i][k]);

				int leftPartSize = static_cast<int>(leftPart.size());
				int rightPartSize = static_cast<int>(rightPart.size());
				int smallerPartSize;
				if (leftPartSize < rightPartSize)
					smallerPartSize = leftPartSize;
				else
					smallerPartSize = rightPartSize;

				bool isPotentialReflection = true;

				if (smudgeX != -1 && (smudgeX < j - smallerPartSize || j + smallerPartSize < smudgeX))
					isPotentialReflection = false;

				for (int k = 0; k < leftPartSize && k < rightPartSize; k++)
				{
					if (leftPart[k] != rightPart[k])
					{
						isPotentialReflection = false;
						break;
					}
				}

				if (i == 0 && isPotentialReflection == true)
					verticalPotentialReflections.push_back(j);			
				else if (i != 0 && isPotentialReflection == false)
					verticalPotentialReflections.erase(std::find(verticalPotentialReflections.begin(), verticalPotentialReflections.end(), j));
			}
		}
	}

	if (verticalPotentialReflections.size() > 0)
	{
		_reflectionIsVertical = true;
		_valueVerticalReflection = verticalPotentialReflections[0];
		_valueReflection = verticalPotentialReflections[0];
	}
}

void Caneva::computeHorizontalReflection(int smudgeY)
{
	std::vector<int> horizontalPotentialReflections;
	for (int i = 0; i < _sizeX; i++)
	{
		for (int j = 1; j < _sizeY; j++)
		{
			if (i == 0 || std::find(horizontalPotentialReflections.begin(), horizontalPotentialReflections.end(), j) != horizontalPotentialReflections.end())
			{
				std::vector<char> topPart;
				std::vector<char> botPart;

				for (int k = j - 1; k >= 0; k--)
					topPart.push_back(_map[k][i]);
				for (int k = j; k < _sizeY; k++)
					botPart.push_back(_map[k][i]);

				int topPartSize = static_cast<int>(topPart.size());
				int botPartSize = static_cast<int>(botPart.size());
				int smallerPartSize;
				if (topPartSize < botPartSize)
					smallerPartSize = topPartSize;
				else
					smallerPartSize = botPartSize;

				bool isPotentialReflection = true;

				if (smudgeY != -1 && (smudgeY < j - smallerPartSize || j + smallerPartSize < smudgeY))
					isPotentialReflection = false;

				for (int k = 0; k < topPartSize && k < botPartSize; k++)
				{
					if (topPart[k] != botPart[k])
					{
						isPotentialReflection = false;
						break;
					}
				}

				if (i == 0 && isPotentialReflection == true)
					horizontalPotentialReflections.push_back(j);			
				else if (i != 0 && isPotentialReflection == false)
					horizontalPotentialReflections.erase(std::find(horizontalPotentialReflections.begin(), horizontalPotentialReflections.end(), j));
			}
		}
	}

	if (horizontalPotentialReflections.size() > 0)
	{
		_reflectionIsHorizontal = true;
		_valueHorizontalReflection = horizontalPotentialReflections[0];
		_valueReflection = horizontalPotentialReflections[0] * 100;
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

int	Caneva::getSmudgeX() const
{
	return (_smudgeX);
}

int	Caneva::getSmudgeY() const
{
	return (_smudgeY);
}

bool	Caneva::reflectionIsVertical() const
{
	return (_reflectionIsVertical);
}

bool	Caneva::reflectionIsHorizontal() const
{
	return (_reflectionIsHorizontal);
}

int	Caneva::getValueVerticalReflection() const
{
	return (_valueVerticalReflection);
}

int	Caneva::getValueHorizontalReflection() const
{
	return (_valueHorizontalReflection);
}

int	Caneva::getValueReflection() const
{
	return (_valueReflection);
}

std::vector<std::vector<char>>	Caneva::getMap() const
{
	return (_map);
}

void resolve(std::vector<Caneva> canevas, int &total, bool debug)
{
	for (std::vector<Caneva>::iterator it = canevas.begin(); it != canevas.end(); it++)
		it->computeSmudge();

	int i = 0;
	for (std::vector<Caneva>::iterator it = canevas.begin(); it != canevas.end(); it++)
	{
		total += it->getValueReflection();

		if (debug == true)
		{
			std::cout << "\033[0mCaneva number \033[0;34m";
			std::cout << i;
			std::cout << "\033[0m of size [\033[0;34m";
			std::cout << it->getSizeX();
			std::cout << "\033[0m/\033[0;34m";
			std::cout << it->getSizeY();
			std::cout << "\033[0m], have a \033[0;32m";
			if (it->reflectionIsVertical() == true)
				std::cout << "vertical";
			else if (it->reflectionIsHorizontal() == true)
				std::cout << "horizontal";
			std::cout << "\033[0m reflection at ";
			if (it->reflectionIsVertical() == true)
			{
				std::cout << "x = \033[0;32m";
				std::cout << it->getValueVerticalReflection();
			}
			else if (it->reflectionIsHorizontal() == true)
			{
				std::cout << "y = \033[0;32m";
				std::cout << it->getValueHorizontalReflection();
			}
			std::cout << "\033[0m, after switching a '\033[0;34m";
			if (it->getMap()[it->getSmudgeY()][it->getSmudgeX()] == '.')
				std::cout << '#';
			else
				std::cout << '.';
			std::cout << "\033[0m' into a '\033[0;34m";
			std::cout << it->getMap()[it->getSmudgeY()][it->getSmudgeX()];
			std::cout << "\033[0m' at [\033[0;34m";
			std::cout << it->getSmudgeX();
			std::cout << "\033[0m/\033[0;34m";
			std::cout << it->getSmudgeY();
			std::cout << "\033[0m], for a value of : \033[0;33m";
			std::cout << it->getValueReflection();
			std::cout << "\033[0m";
			std::cout << std::endl;
			for (int i = 0; i < it->getSizeY(); i++)
			{
				for (int j = 0; j < it->getSizeX(); j++)
				{
					if (it->reflectionIsHorizontal() == true)
					{
						if (i >= it->getValueHorizontalReflection())
							std::cout << "\033[0;35m";
						else
							std::cout << "\033[0;36m";
					}
					else if (it->reflectionIsVertical() == true)
					{
						if (j >= it->getValueVerticalReflection())
							std::cout << "\033[0;35m";
						else
							std::cout << "\033[0;36m";
					}
					if (i == it->getSmudgeY() && j == it->getSmudgeX())
						std::cout << "\033[0;32m";
					std::cout << it->getMap()[i][j];
				}
				std::cout << std::endl;
			}
			std::cout << "\033[0m";
			std::cout << std::endl;
		}
		i++;
	}
}

void pars(std::string line, std::vector<Caneva> &canevas)
{
	if (line == "")
		canevas.push_back(Caneva());
	else
		canevas.back().addLine(line);
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
	std::vector<Caneva> canevas;
	int total = 0;
	bool debug = false;

	if (argc == 3)
		debug = true;

	canevas.push_back(Caneva());
	while(std::getline(input, line))
		pars(line, canevas);

	resolve(canevas, total, debug);

	std::cout << "The number get after summarizing all the notes is : \033[1;33m";
	std::cout << total;
	std::cout << "\033[0m" << std::endl;
	return (0);
}