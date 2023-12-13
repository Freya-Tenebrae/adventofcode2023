#include <string>
#include <fstream>
#include <iostream>
#include <vector>

void getNumbers(std::vector<int> &n, std::string s)
{
	for(size_t i = 0; i < s.length(); i++)
	{
		if (s[i] != ' ')
		{
			size_t pos = s.find(" ", i);
			if (pos == std::string::npos)
				pos = s.length() - 1;

			n.push_back(std::stoi(s.substr(i, pos)));

			while(s[i] && s[i] >= '0' && s[i] <= '9')
				i++;
		}
	}
}

int getValueCard(std::vector<int> winningNumbers, std::vector<int> myNumbers)
{
	int matchFound = 0;

	for(std::vector<int>::iterator it = myNumbers.begin(); it != myNumbers.end(); ++it)
	{
		for(std::vector<int>::iterator it2 = winningNumbers.begin(); it2 != winningNumbers.end(); ++it2)
		{
			if (*it == *it2)
				matchFound++;
		}
	}

	if (matchFound == 0)
		return 0;

	return matchFound;
}

void resolve(std::string line, std::vector<int> &numberCardsGet, bool debug)
{
	std::string card, winningNumbersString, myNumbersString;
	int numberCard;
	std::vector<int> winningNumbers, myNumbers;
	int cardValue = 0;

	card = line.substr(0, line.find(":"));
	card.erase(0, line.find(" "));
	numberCard = std::stoi(card);

	if (static_cast<size_t>(numberCard) > numberCardsGet.size())
		numberCardsGet.push_back(1);

	line.erase(0, line.find(":") + 2);
	winningNumbersString = line.substr(0, line.find("|") - 1);
	line.erase(0, line.find("|") + 2);
	myNumbersString = line.substr();

	getNumbers(winningNumbers, winningNumbersString);
	getNumbers(myNumbers, myNumbersString);

	cardValue = getValueCard(winningNumbers, myNumbers);

	for (int i = 1; i <= cardValue; i++)
	{
		if (static_cast<size_t>(numberCard + cardValue) > numberCardsGet.size())
			numberCardsGet.push_back(1);

		numberCardsGet[numberCard + i - 1] += 1 * numberCardsGet[numberCard - 1];
	}

	if (debug == true)
	{
		std::cout << "Card: \033[0;34m" << numberCard;
		std::cout << " \033[0m, Winning numbers: \033[0;30m";
		for (size_t i = 0; i < winningNumbers.size(); i++)
			std::cout << winningNumbers[i] << " ";
		std::cout << "\033[0m, My numbers: \033[0;30m";
		for (size_t i = 0; i < myNumbers.size(); i++)
			std::cout << myNumbers[i] << " ";
		std::cout << "\033[0m, Card Value = \033[0;34m" << cardValue << "\033[0m";
		std::cout << "\033[0m, Number of instances = \033[0;32m" << numberCardsGet[numberCard - 1] << "\033[0m";
		std::cout << std::endl;
	}
}

int getTotal(std::vector<int> numberCardsGet)
{
	int total = 0;

	for (std::vector<int>::iterator it = numberCardsGet.begin(); it != numberCardsGet.end(); ++it)
		total += *it;

	return total;
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
	std::vector<int> numberCardsGet;

	if (argc == 3)
		debug = true;

	numberCardsGet.push_back(1);

	while(std::getline(input, line))
		resolve(line, numberCardsGet, debug);

	total = getTotal(numberCardsGet);

	std::cout << "The total of scratchcards i end up with is : \033[1;33m";
	std::cout << total;
	std::cout << "\033[0m" << std::endl;
	return (0);
}