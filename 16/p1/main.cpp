#include <string>
#include <fstream>
#include <iostream>
#include <vector>

class Beam
{
	private:
		int		_posX;
		int		_posY;
		char	_direction; // 'E' "-->" , 'S' "\/" , 'W' "<--" , 'N' "/\"
		bool	_isOver;

	public:
		Beam(int posX, int posY, int direction);
		~Beam();

		int getPosX();
		int getPosY();
		int getDirection();

		void move();
		void bounce(char mirrorType); // 1 : '/', 2 : '\'
		Beam *split(char splitterType); // 1 : '|', 2 : '-'
		void BeamIsOver(int sizeMapX, int sizeMapY);
		bool isBeamOver();
};

Beam::Beam(int posX, int posY, int direction): _posX(posX), _posY(posY), _direction(direction), _isOver(false)
{

}

Beam::~Beam()
{

}

int Beam::getPosX()
{
	return _posX;
}

int Beam::getPosY()
{
	return _posY;
}

int Beam::getDirection()
{
	return _direction;
}

void Beam::move()
{
	if (_direction == 'E')
		_posX++;
	else if (_direction == 'S')
		_posY++;
	else if (_direction == 'W')
		_posX--;
	else if (_direction == 'N')
		_posY--;
}

void Beam::bounce(char mirrorType)
{
	if (mirrorType == '/')
	{
		if (_direction == 'E')
			_direction = 'N';
		else if (_direction == 'S')
			_direction = 'W';
		else if (_direction == 'W')
			_direction = 'S';
		else if (_direction == 'N')
			_direction = 'E';
	}
	else if (mirrorType == '\\')
	{
		if (_direction == 'E')
			_direction = 'S';
		else if (_direction == 'S')
			_direction = 'E';
		else if (_direction == 'W')
			_direction = 'N';
		else if (_direction == 'N')
			_direction = 'W';
	}
}

Beam *Beam::split(char splitterType)
{
	if (splitterType == '|' && (_direction == 'E' || _direction == 'W'))
	{
		_direction = 'S';
		Beam *newBeam = new Beam(_posX, _posY, 'N');
		return newBeam;
	}
	else if (splitterType == '-' && (_direction == 'N' || _direction == 'S'))
	{
		_direction = 'E';
		Beam *newBeam = new Beam(_posX, _posY, 'W');
		return newBeam;
	}

	return NULL;
}

void Beam::BeamIsOver(int sizeMapX, int sizeMapY)
{
	if (_posX < 0 || _posX >= sizeMapX || _posY < 0 || _posY >= sizeMapY)
		_isOver = true;
}

bool Beam::isBeamOver()
{
	return _isOver;
}

class Cave
{
	private:
		std::vector<std::vector<char>> _map;
		std::vector<std::vector<bool>> _isEnergized;
		std::vector<Beam *> _beams;
		std::vector<std::vector<int>> _splitterHaveBennUsed;

	public:
		Cave();
		~Cave();

		void addLine(std::string line);
		void addBeam(Beam *beam);
		void moveBeams();
		void start();
		bool endOfBeams();

		int getNumberTileEnergized();
		void printMap();
		void printBeams();
};

Cave::Cave()
{

}

Cave::~Cave()
{

}

void Cave::addLine(std::string line)
{
	std::vector<char> row;
	std::vector<bool> isEnergized;

	for (std::string::iterator it = line.begin(); it != line.end(); ++it)
	{
		row.push_back(*it);
		isEnergized.push_back(false);
	}
	_map.push_back(row);
	_isEnergized.push_back(isEnergized);
}

void Cave::addBeam(Beam *newBeam)
{
	if (newBeam != NULL)
		_beams.push_back(newBeam);
}

bool Cave::endOfBeams()
{
	for (size_t i = 0; i < _beams.size(); ++i)
	{
		if (_beams[i] != NULL && _beams[i]->isBeamOver() == false)
			return false;
	}

	return true;
}

void Cave::moveBeams()
{
	std::vector<Beam *> newBeams;

	for (size_t i = 0; i < _beams.size(); ++i)
	{
		if (_beams[i] != NULL)
		{
			_beams[i]->BeamIsOver(_map[0].size(), _map.size());
			if (_beams[i]->isBeamOver() == false)
			{
				_isEnergized[_beams[i]->getPosY()][_beams[i]->getPosX()] = true;

				if (_map[_beams[i]->getPosY()][_beams[i]->getPosX()] == '/' || _map[_beams[i]->getPosY()][_beams[i]->getPosX()] == '\\')
					_beams[i]->bounce(_map[_beams[i]->getPosY()][_beams[i]->getPosX()]);
				else if (_map[_beams[i]->getPosY()][_beams[i]->getPosX()] == '|' || _map[_beams[i]->getPosY()][_beams[i]->getPosX()] == '-')
				{
					bool haveBeenUsed = false;
					for (std::vector<std::vector<int>>::iterator it = _splitterHaveBennUsed.begin(); it != _splitterHaveBennUsed.end(); ++it)
					{
						if ((*it)[0] == _beams[i]->getPosX() && (*it)[1] == _beams[i]->getPosY())
						{
							haveBeenUsed = true;
							break;
						}
					}
					if (haveBeenUsed == false)
					{
						if ((_map[_beams[i]->getPosY()][_beams[i]->getPosX()] == '|' && (_beams[i]->getDirection() == 'E' || _beams[i]->getDirection() == 'W'))|| \
							(_map[_beams[i]->getPosY()][_beams[i]->getPosX()] == '-' && (_beams[i]->getDirection() == 'N' || _beams[i]->getDirection() == 'S')))
						{
							_splitterHaveBennUsed.push_back({ _beams[i]->getPosX(), _beams[i]->getPosY() });
							newBeams.push_back(_beams[i]->split(_map[_beams[i]->getPosY()][_beams[i]->getPosX()]));
						}
					}
					else if (haveBeenUsed == true)
					{
						delete _beams[i];
						_beams[i] = NULL;
					}
				}
			}
			else
			{
				delete _beams[i];
				_beams[i] = NULL;
			}
		}
	}

	for (size_t i = 0; i < newBeams.size(); ++i)
		addBeam(newBeams[i]);

	for (size_t i = 0; i < _beams.size(); ++i)
	{
		if (_beams[i] != NULL)
			_beams[i]->move();
	}
}

void Cave::start()
{
	addBeam(new Beam(0, 0, 'E'));
	while(endOfBeams() == false)
	{
		moveBeams();
	}
}

int Cave::getNumberTileEnergized()
{
	int value = 0;

	for (std::vector<std::vector<bool>>::iterator it = _isEnergized.begin(); it != _isEnergized.end(); ++it)
	{
		for (std::vector<bool>::iterator it2 = (*it).begin(); it2 != (*it).end(); ++it2)
		{
			if (*it2 == true)
				value++;
		}
	}

	return value;
}

void Cave::printMap()
{
	for (std::vector<std::vector<char>>::iterator it = _map.begin(); it != _map.end(); ++it)
	{
		for (std::vector<char>::iterator it2 = (*it).begin(); it2 != (*it).end(); ++it2)
		{
			if (*it2 == '.')
				std::cout << "\033[0;30m" << *it2;
			else
				std::cout << "\033[0;34m" << *it2;
		}
		std::cout << std::endl;
	}
	std::cout << "\033[0m";
	std::cout << std::endl;
}

void Cave::printBeams()
{
	for (std::vector<std::vector<bool>>::iterator it = _isEnergized.begin(); it != _isEnergized.end(); ++it)
	{
		for (std::vector<bool>::iterator it2 = (*it).begin(); it2 != (*it).end(); ++it2)
		{
			if (*it2 == true)
				std::cout << "\033[1;31m" << "#";
			else
				std::cout << "\033[0;30m" << ".";
		}
		std::cout << std::endl;
	}
	std::cout << "\033[0m";
	std::cout << std::endl;
}

void resolve(Cave cave, int &total, bool debug)
{
	cave.start();
	if (debug)
	{
		cave.printMap();
		cave.printBeams();
	}

	total = cave.getNumberTileEnergized();;
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
	Cave cave;

	if (argc == 3)
		debug = true;

	while(std::getline(input, line))
		cave.addLine(line);

	resolve(cave, total, debug);

	std::cout << "The number of tiles being energized is : \033[1;33m";
	std::cout << total;
	std::cout << "\033[0m" << std::endl;
	return (0);
}