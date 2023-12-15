#include <string>
#include <fstream>
#include <iostream>
#include <vector>

class Cypher
{
	private :
		std::vector<std::string>	_cryptedMessage;
		std::vector<int>			_convertedMessage;
	
	public:
		Cypher();
		~Cypher();

		void 	addCryptedLetter(std::string cryptedLetter);
		void 	convert(bool debug);
		int		getValueconvertedMessage() const;
};

Cypher::Cypher()
{

}

Cypher::~Cypher()
{

}

void Cypher::addCryptedLetter(std::string cryptedLetter)
{
	_cryptedMessage.push_back(cryptedLetter);
}

void Cypher::convert(bool debug)
{
	for (size_t i = 0; i < _cryptedMessage.size(); i++)
	{
		int valueLetter = 0;
		for (size_t j = 0; j < _cryptedMessage[i].size(); j++)
		{
			valueLetter += _cryptedMessage[i][j];
			valueLetter *= 17;
			valueLetter %= 256;
		}

		_convertedMessage.push_back(valueLetter);

		if (debug)
		{
			std::cout << "Crypted letter : \033[0;34m";
			std::cout << _cryptedMessage[i];
			std::cout << "\033[0m, is converted into \033[0;32m";
			std::cout << _convertedMessage[i];
			std::cout << "\033[0m, or \033[0;32m";
			std::cout << static_cast<char>(_convertedMessage[i]);
			std::cout << "\033[0m after being converted into a char\033[0m.";
			std::cout << std::endl;
		}
	}
}

int Cypher::getValueconvertedMessage() const
{
	int valueAllLetter = 0;
	for (size_t i = 0; i < _convertedMessage.size(); i++)
		valueAllLetter += _convertedMessage[i];

	return (valueAllLetter);
}

void pars(std::string line, Cypher &cypher)
{
	std::string cryptedLetter;

	for (size_t i = 0; i < line.size(); i++)
	{
		if (line[i] == ',')
		{
			cypher.addCryptedLetter(cryptedLetter);
			cryptedLetter.clear();
		}
		else
			cryptedLetter += line[i];
	}
	cypher.addCryptedLetter(cryptedLetter);
}

void resolve(Cypher cypher, int &total, bool debug)
{
	cypher.convert(debug);
	total = cypher.getValueconvertedMessage();
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
	Cypher cypher;

	if (argc == 3)
		debug = true;

	while(std::getline(input, line))
		pars(line, cypher);

	resolve(cypher, total, debug);

	std::cout << "The number of line is : \033[1;33m";
	std::cout << total;
	std::cout << "\033[0m" << std::endl;
	return (0);
}