#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

class Cypher
{
	private :
		std::string	_cryptedLetter;
		int			_convertedLetter;
	
	public:
		Cypher(std::string cryptedLetter);
		~Cypher();

		int		getValueconvertedLetter() const;
};

Cypher::Cypher(std::string cryptedLetter): _cryptedLetter(cryptedLetter), _convertedLetter(0)
{
	for (size_t i = 0; i < _cryptedLetter.length(); i++)
	{
		_convertedLetter += _cryptedLetter[i];
		_convertedLetter *= 17;
		_convertedLetter %= 256;
	}
}

Cypher::~Cypher()
{

}

int Cypher::getValueconvertedLetter() const
{
	return (_convertedLetter);
}

class Lens
{
	private:
		std::string	_label;
		int			_focalLength;

	public:
		Lens(std::string label, int focalLength);
		~Lens();

		std::string	getLabel() const;
		int					getFocalLength() const;

		void 				setFocalLength(int focalLength);
};

Lens::Lens(std::string label, int focalLength): _label(label), _focalLength(focalLength)
{

}

Lens::~Lens()
{

}

std::string	Lens::getLabel() const
{
	return (_label);
}

int	Lens::getFocalLength() const
{
	return (_focalLength);
}

void Lens::setFocalLength(int focalLength)
{
	_focalLength = focalLength;

}

class Box
{
	private:
		int					_number;
		std::vector<Lens>	_lenses;

		bool	isLensPresent(std::string label) const;
		int		getLensPosition(std::string label) const;

	public:
		Box(int value);
		~Box();

		void	addLens(std::string label, int focalLength);
		void	removeLens(std::string label);

		void	showBox() const;
		int		getNumberBox() const;
		int 	getValueBox() const;
};

Box::Box(int number): _number(number)
{

}

Box::~Box()
{

}

bool	Box::isLensPresent(std::string label) const
{
	for (std::vector<Lens>::const_iterator it = _lenses.begin(); it != _lenses.end(); it++)
	{
		if ((*it).getLabel() == label)
			return (true);
	}
	return (false);
}

int	Box::getLensPosition(std::string label) const
{
	for (size_t i = 0; i < _lenses.size(); i++)
	{
		if (_lenses[i].getLabel() == label)
			return (i);
	}
	return (-1);

}

void	Box::addLens(std::string label, int focalLength)
{
	if (isLensPresent(label) == false)
		_lenses.push_back(Lens(label, focalLength));
	else
		_lenses[getLensPosition(label)].setFocalLength(focalLength);
}

void	Box::removeLens(std::string label)
{
	if (isLensPresent(label) == true)
		_lenses.erase(_lenses.begin() + getLensPosition(label));
}

int Box::getValueBox() const
{
	int value = 0;
	for (size_t i = 0; i < _lenses.size(); i++)
		value += _lenses[i].getFocalLength() * (i + 1);

	return value;
}

void	Box::showBox() const
{
	std::cout << "Box \033[0;34m";
	std::cout << _number;
	std::cout << "\033[0m :";
	for (std::vector<Lens>::const_iterator it = _lenses.begin(); it != _lenses.end(); it++)
	{
		std::cout << " [ \033[0;34m";
		std::cout << (*it).getLabel();
		std::cout << "\033[0m \033[0;34m";
		std::cout << (*it).getFocalLength();
		std::cout << "\033[0m ]";
	}
	std::cout << std::endl;
}

int	Box::getNumberBox() const
{
	return (_number);
}

class Line
{
	private:
		std::vector<Box>	_boxes;

	public:
		Line();
		~Line();

		void	operationAddingLens(std::string label, int focalLength);
		void	operationRemovingLens(std::string label);

		void	showLine() const;
		int		getValueLine() const;
};

Line::Line()
{
	for (int i = 0; i < 256; i++)
		_boxes.push_back(Box(i));
}

Line::~Line()
{

}

void	Line::operationAddingLens(std::string label, int focalLength)
{
	Cypher cypher(label);

	_boxes[cypher.getValueconvertedLetter()].addLens(label, focalLength);
}

void	Line::operationRemovingLens(std::string label)
{
	Cypher cypher(label);

	_boxes[cypher.getValueconvertedLetter()].removeLens(label);
}

void	Line::showLine() const
{
	for (std::vector<Box>::const_iterator it = _boxes.begin(); it != _boxes.end(); it++)
	{
		if (it->getValueBox() != 0)
			it->showBox();
	}

	for (std::vector<Box>::const_iterator it = _boxes.begin(); it != _boxes.end(); it++)
	{
		if (it->getValueBox() != 0)
		{
			std::cout << "Box \033[0;34m";
			std::cout << it->getNumberBox();
			std::cout << "\033[0m has a value of \033[0;32m";
			std::cout << it->getValueBox();
			std::cout << "\033[0m" << std::endl;
		}
	}
}

int		Line::getValueLine() const
{
	int value = 0;

	for (std::vector<Box>::const_iterator it = _boxes.begin(); it != _boxes.end(); it++)
		value += it->getValueBox() * (it->getNumberBox() + 1);

	return (value);
}

void pars(std::string line, std::vector<std::string> &HASHMAP)
{
	std::string actionHASHMAP;

	for (size_t i = 0; i < line.size(); i++)
	{
		if (line[i] == ',')
		{
			HASHMAP.push_back(actionHASHMAP);
			actionHASHMAP.clear();
		}
		else
			actionHASHMAP += line[i];
	}
	HASHMAP.push_back(actionHASHMAP);
}

void resolve(std::vector<std::string> HASHMAP, int &total, bool debug)
{
	Line line;

	for (size_t i = 0; i < HASHMAP.size(); i++)
	{
		std::string label;
		for (size_t j = 0; j < HASHMAP[i].size(); j++)
		{
			if (HASHMAP[i][j] != '=' && HASHMAP[i][j] != '-')
				label+= HASHMAP[i][j];
			else
			{
				if (HASHMAP[i][j] == '=')
					line.operationAddingLens(label, HASHMAP[i][j + 1] - '0');
				else if (HASHMAP[i][j] == '-')
					line.operationRemovingLens(label);
				break;
			}
		}
	}

	total = line.getValueLine();

	if (debug)
		line.showLine();
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
	std::vector<std::string> HASHMAP;

	if (argc == 3)
		debug = true;

	while(std::getline(input, line))
		pars(line, HASHMAP);

	resolve(HASHMAP, total, debug);

	std::cout << "The number of line is : \033[1;33m";
	std::cout << total;
	std::cout << "\033[0m" << std::endl;
	return (0);
}