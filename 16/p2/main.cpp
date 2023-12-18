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
		int _sizeX;
		int _sizeY;
		std::vector<std::vector<char>> _map;
		std::vector<std::vector<bool>> _isEnergized;
		std::vector<Beam *> 		   _beams;
		std::vector<std::vector<int>>  _splitterHaveBennUsed;
		int 						   _numberTileEnergized;

	public:
		Cave();
		Cave(const Cave &cave);
		~Cave();

		Cave &operator=(const Cave &cave);

		void addLine(std::string line);
		void addBeam(Beam *beam);
		void moveBeams();
		void start(int x, int y, char direction);
		bool endOfBeams();

		void computeNumberTileEnergized();
		int getNumberTileEnergized() const;
		int getSizeX() const;
		int getSizeY() const;
		void printMap() const;
		void printBeams() const;

		std::vector<std::vector<char>> getMap() const;
		std::vector<std::vector<bool>> getIsEnergized() const;
};

Cave::Cave():_sizeX(0), _sizeY(0), _numberTileEnergized(0)
{

}

Cave::Cave(const Cave &cave)
{
	(*this) = cave;
}

Cave::~Cave()
{

}

Cave &Cave::operator=(const Cave &cave)
{
	_sizeX = cave.getSizeX();
	_sizeY = cave.getSizeY();
	_map = cave.getMap();
	_isEnergized = cave.getIsEnergized();

	return (*this);
}

void Cave::addLine(std::string line)
{
	std::vector<char> row;
	std::vector<bool> isEnergized;

	if (_map.size() == 0)
		_sizeX = line.size();

	for (std::string::iterator it = line.begin(); it != line.end(); ++it)
	{
		row.push_back(*it);
		isEnergized.push_back(false);
	}
	_sizeY++;
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

void Cave::start(int x, int y, char direction)
{
	addBeam(new Beam(x, y, direction));
	while(endOfBeams() == false)
	{
		moveBeams();
	}
	computeNumberTileEnergized();
}

void Cave::computeNumberTileEnergized()
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
	_numberTileEnergized = value;
}

int Cave::getNumberTileEnergized() const
{
	return _numberTileEnergized;
}

int Cave::getSizeX() const
{
	return _sizeX;
}

int Cave::getSizeY() const
{
	return _sizeY;
}

void Cave::printMap() const
{
	for (std::vector<std::vector<char>>::const_iterator it = _map.begin(); it != _map.end(); ++it)
	{
		for (std::vector<char>::const_iterator it2 = (*it).begin(); it2 != (*it).end(); ++it2)
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

void Cave::printBeams() const
{
	for (std::vector<std::vector<bool>>::const_iterator it = _isEnergized.begin(); it != _isEnergized.end(); ++it)
	{
		for (std::vector<bool>::const_iterator it2 = (*it).begin(); it2 != (*it).end(); ++it2)
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

std::vector<std::vector<char>> Cave::getMap() const
{
	return _map;
}
std::vector<std::vector<bool>> Cave::getIsEnergized() const
{
	return _isEnergized;
}

class Configuration
{
	private:
		int _sizeMapX;
		int _sizeMapY;
		Cave _baseCave;
		std::vector<Cave> _caveConfiguration;

	public:
		Configuration(Cave &cave);
		~Configuration();

		void	addCave(Cave cave);
		void	ConfigurationShowConfiguration(int n, int x, int y, char direction);
		void	computeCaveConfigurations(bool debug);
		int		getBetterCaveConfiguration(bool debug);
};

Configuration::Configuration(Cave &cave)
{
	_baseCave = Cave(cave);
}

Configuration::~Configuration()
{

}

void	Configuration::addCave(Cave cave)
{
	if (_caveConfiguration.size() == 0)
	{
		_sizeMapX = cave.getSizeX();
		_sizeMapY = cave.getSizeY();
	}
	_caveConfiguration.push_back(cave);

}

void Configuration::ConfigurationShowConfiguration(int n, int x, int y, char direction)
{
	std::cout << "\033[0mCave configuration number \033[0;34m";
	std::cout << n;
	std::cout << "\033[0m that start at [\033[0;34m";
	std::cout << y;
	std::cout << "\033[0m/\033[0;34m";
	std::cout << x;
	std::cout << "\033[0m] to \033[0;34m";
	if (direction == 'E')
		std::cout << "East";
	else if (direction == 'S')
		std::cout << "South";
	else if (direction == 'W')
		std::cout << "West";
	else
		std::cout << "North";
	std::cout << "\033[0m have \033[0;32m";
	std::cout << _caveConfiguration.back().getNumberTileEnergized();
	std::cout << "\033[0m energized tiles";
	std::cout << std::endl;
}

void	Configuration::computeCaveConfigurations(bool debug)
{
	for (int i = 0; i < _sizeMapX; ++i)
	{
		addCave(_baseCave);
		_caveConfiguration.back().start(i, 0, 'S');
		if (debug)
			ConfigurationShowConfiguration(_caveConfiguration.size(), i, 0, 'S');
		addCave(_baseCave);
		_caveConfiguration.back().start(i, _sizeMapY - 1, 'N');
		if (debug)
			ConfigurationShowConfiguration(_caveConfiguration.size(), i, _sizeMapY - 1, 'N');
	}

	for (int i = 0; i < _sizeMapY; ++i)
	{
		addCave(_baseCave);
		_caveConfiguration.back().start(0, i, 'E');
		if (debug)
			ConfigurationShowConfiguration(_caveConfiguration.size(), 0, i, 'E');
		addCave(_baseCave);
		_caveConfiguration.back().start(_sizeMapX - 1, i, 'W');
		if (debug)
			ConfigurationShowConfiguration(_caveConfiguration.size(), _sizeMapX - 1, i, 'W');
	}
}

int	Configuration::getBetterCaveConfiguration(bool debug)
{
	int betterCaveConfiguration = 0;
	for (size_t i = 1; i < _caveConfiguration.size(); ++i)
	{
		if (_caveConfiguration[betterCaveConfiguration].getNumberTileEnergized() < _caveConfiguration[i].getNumberTileEnergized())
			betterCaveConfiguration = i;
	}
	if (debug)
	{
		std::cout << std::endl;
		std::cout << "The better cave configuration is :";
		std::cout << std::endl;
		if (betterCaveConfiguration < 2 * _sizeMapX)
		{
			if (betterCaveConfiguration % 2 == 0)
				ConfigurationShowConfiguration(betterCaveConfiguration, betterCaveConfiguration / 2, 0, 'S');
			else
				ConfigurationShowConfiguration(betterCaveConfiguration, betterCaveConfiguration / 2, _sizeMapY - 1, 'N');
		}
		else
		{
			if (betterCaveConfiguration % 2 == 0)
				ConfigurationShowConfiguration(betterCaveConfiguration, 0, (betterCaveConfiguration - _sizeMapX * 2) / 2, 'E');
			else
				ConfigurationShowConfiguration(betterCaveConfiguration, _sizeMapX - 1, (betterCaveConfiguration - _sizeMapX * 2) / 2, 'W');
		}
		_baseCave.printMap();
		std::cout << std::endl;
		_caveConfiguration[betterCaveConfiguration].printBeams();
		std::cout << std::endl;
	}

	return _caveConfiguration[betterCaveConfiguration].getNumberTileEnergized();
}


void resolve(Configuration configuration, int &total, bool debug)
{
	configuration.computeCaveConfigurations(debug);
	total = configuration.getBetterCaveConfiguration(debug);
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

	Configuration configuration(cave);

	resolve(configuration, total, debug);

	std::cout << "The number of tiles being energized for the better cave configuration is : \033[1;33m";
	std::cout << total;
	std::cout << "\033[0m" << std::endl;
	return (0);
}