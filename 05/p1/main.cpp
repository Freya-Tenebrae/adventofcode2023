#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <climits>

class Conversion 
{
	private:
		const long _destinationRange;
		const long _sourceRange;
		const long _rangeLength;

	public:
		Conversion(long destinationRange, long sourceRange, long rangeLength);
		~Conversion();

		bool isInSourceRange(long value) const;
		long convert(long value) const;
};

class Converter
{
	private:
		const std::string _converterName;
		std::vector<Conversion> _conversions;

	public:
		Converter(std::string converterName);
		~Converter();

		std::string getConverterName() const;

		void addConversion(long destinationRange, long sourceRange, long rangeLength);

		long convert(long value) const;
};

Conversion::Conversion(long destinationRange, long sourceRange, long rangeLength):
	_destinationRange(destinationRange),
	_sourceRange(sourceRange),
	_rangeLength(rangeLength)
{

}

Conversion::~Conversion()
{

}

bool Conversion::isInSourceRange(long value) const
{
	if (value >= _sourceRange && value < _sourceRange + _rangeLength)
		return (true);
	return (false);
}

long Conversion::convert(long value) const
{
	return (_destinationRange + (value - _sourceRange));
}

Converter::Converter(std::string converterName):
	_converterName(converterName)
{

}

Converter::~Converter()
{

}

std::string Converter::getConverterName() const
{
	return (_converterName);

}

void Converter::addConversion(long destinationRange, long sourceRange, long rangeLength)
{
	Conversion conversion(destinationRange, sourceRange, rangeLength);
	_conversions.push_back(conversion);
}

long Converter::convert(long value) const
{
	for (std::vector<Conversion>::const_iterator it = _conversions.begin(); it != _conversions.end(); ++it)
	{
		if (it->isInSourceRange(value) == true)
			return (it->convert(value));
	}
	return (value);
}

void parsFirstLine(std::string line, std::vector<long> &seeds)
{
	line.erase(0, line.find(":") + 1);
	for(size_t i = 0; i < line.length(); i++)
	{
		if (line[i] != ' ')
		{
			size_t pos = line.find(" ", i);
			if (pos == std::string::npos)
				pos = line.length();
			seeds.push_back(std::stol(line.substr(i, pos - i)));

			while(line[i] && line[i] >= '0' && line[i] <= '9')
				i++;
		}
	}
}

void pars(std::string line, std::vector<Converter> &conversions)
{
	if (line.length() != 0 && line[0] >= '0' && line[0] <= '9')
	{
		long destinationRange = std::stol(line.substr(0, line.find(" ")));
		line.erase(0, line.find(" ") + 1);
		long sourceRange = std::stol(line.substr(0, line.find(" ")));
		line.erase(0, line.find(" ") + 1);
		long rangeLength = std::stol(line.substr(0));

		conversions.back().addConversion(destinationRange, sourceRange, rangeLength);
	}
	else if (line.length() != 0)
	{
		line.erase(line.find(":") - 4, line.find(":"));
		conversions.push_back(Converter(line));
	}
}

void resolve(bool debug, long &total, std::vector<long> &seeds, std::vector<Converter> &converter)
{
	for (std::vector<long>::iterator it = seeds.begin(); it != seeds.end(); ++it)
	{
		long value = *it;
		if (debug == true)
			std::cout << "seed: \033[0;34m" << value << "\033[0;30m -";
		for (std::vector<Converter>::iterator it2 = converter.begin(); it2 != converter.end(); ++it2)
		{
			value = it2->convert(value);
			if (debug == true)
				std::cout << it2->getConverterName() << "-> \033[0;34m" << value << " \033[0;30m-";
		}
		if (debug == true)
			std::cout << ">\033[0;32m " << value << " >\033[0m ";

		if (value < total)
		{
			total = value;
			if (debug == true)
				std::cout << " \033[0;33mNEW LOWEST LOCATION\033[0m";
		}

		if (debug == true)
			std::cout << std::endl;
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
	long total = INT_MAX;
	bool debug = false;

	std::vector<long> seeds;
	std::vector<Converter> converter;

	if (argc == 3)
		debug = true;

	std::getline(input, line);
	parsFirstLine(line, seeds);

	while(std::getline(input, line))
	{
		if (line != "")
			pars(line, converter);
	}

	resolve(debug, total, seeds, converter);

	std::cout << "the lowest location number that corresponds to any of the seed numbers is : \033[1;33m";
	std::cout << total;
	std::cout << "\033[0m" << std::endl;
	return (0);
}