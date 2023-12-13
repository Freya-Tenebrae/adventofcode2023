#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>

class Hand 
{
	private:
		std::string _hand;
		int _value;
		std::map<char, int> _power;

	public:
		Hand(std::string hand, int value);
		~Hand();

		std::string getHand() const;
		int getValue() const;
		std::map<char, int> getPower() const;
};

Hand::Hand(std::string hand, int value)
{
	_hand = hand;
	_value = value;

	for (std::string::iterator it = _hand.begin(); it != _hand.end(); it++)
	{
		if (_power.find(*it) == _power.end())
			_power.insert(std::pair<char, int>(*it, 1));
		else
			_power[*it]++;
	}
}

Hand::~Hand()
{

}

std::string Hand::getHand() const
{
	return _hand;
}

int Hand::getValue() const
{
	return _value;
}

std::map<char, int> Hand::getPower() const
{
	return _power;
}

static std::map<char, int>::const_iterator getMaximumNUmberOfSameCard(std::map<char, int> &power)
{
	return max_element(power.begin(), power.end(), \
		[] (const std::pair<char, int> & p1, const std::pair<char, int> & p2)
		{
			return p1.second < p2.second;
		});
}

static bool otherPairPresent(std::map<char, int> &power, std::map<char, int>::const_iterator it)
{
	for (std::map<char, int>::const_iterator it2 = power.begin(); it2 != power.end(); it2++)
	{
		if (it2->first != it->first && it2->second == 2)
			return true;
	}
	return false;
}

static int getCardValue(char card)
{
	if (card <= '9' && card >= '2')
		return card - '0';
	else if (card == 'T')
		return 10;
	else if (card == 'J')
		return 11;
	else if (card == 'Q')
		return 12;
	else if (card == 'K')
		return 13;
	else if (card == 'A')
		return 14;
	else
		return 0;
}

// static int getHigherCardValue(std::map<char, int> &power, std::string &exeption)
// {
// 	std::map<char, int>::const_iterator itTarget = power.end();
// 	int value = 0;
// 	int cardValue = 0;

// 	for (std::map<char, int>::const_iterator it = power.begin(); it != power.end(); it++)
// 	{
// 		if (exeption.find(it->first) == std::string::npos && value < it->second)
// 		{
// 			value = it->second;
// 			cardValue = getCardValue(it->first);
// 			itTarget = it;
// 		}
// 		else if (exeption.find(it->first) == std::string::npos && value == it->second && cardValue < getCardValue(it->first))
// 		{
// 			cardValue = getCardValue(it->first);
// 			itTarget = it;
// 		}
// 	}

// 	if (itTarget == power.end())
// 		return 0;
	
// 	exeption += itTarget->first;
// 	return cardValue;
// }

bool winAgainst(Hand &myHand, Hand &foeHand) 
{
	std::map<char, int> myPower = myHand.getPower();
	std::map<char, int> foePower = foeHand.getPower();

	std::map<char, int>::const_iterator it_me = getMaximumNUmberOfSameCard(myPower);
	std::map<char, int>::const_iterator it_foe = getMaximumNUmberOfSameCard(foePower);

	std::string myExeption = "";
	std::string foeExeption = "";
	
	if (it_me->second > it_foe->second)
		return true;
	else if (it_me->second < it_foe->second)
		return false;
	else if (it_me->second == 3 || it_me->second == 2)
	{
		bool iHaveOneMorePair = otherPairPresent(myPower, it_me);
		bool foeHaveOneMorePair = otherPairPresent(foePower, it_foe);

		if (iHaveOneMorePair == true && foeHaveOneMorePair == false)
			return true;
		else if (iHaveOneMorePair == false && foeHaveOneMorePair == true)
			return false;
	}

	// int myHighercard = getHigherCardValue(myPower, myExeption);
	// int foeHighercard = getHigherCardValue(foePower, foeExeption);

	// while (myHighercard != 0 && foeHighercard != 0)
	// {
	// 	if (myHighercard > foeHighercard)
	// 		return true;
	// 	else if (myHighercard < foeHighercard)
	// 		return false;
	// 	myHighercard = getHigherCardValue(myPower, myExeption);
	// 	foeHighercard = getHigherCardValue(foePower, foeExeption);
	// }

	for (size_t i = 0; i < 5; i++)
	{
		if (getCardValue(myHand.getHand()[i]) > getCardValue(foeHand.getHand()[i]))
			return true;
		else if (getCardValue(myHand.getHand()[i]) < getCardValue(foeHand.getHand()[i]))
			return false;
	}

	std::cout << "ERROR : HANDS HAVE THE SAME VALUE :" << std::endl << "|";
	for (std::map<char, int>::iterator it = myPower.begin(); it != myPower.end(); it++)
		std::cout << it->first << "," << it->second << " | ";
	std::cout << std::endl << "|";
	for (std::map<char, int>::iterator it = foePower.begin(); it != foePower.end(); it++)
		std::cout << it->first << "," << it->second << " | ";
	std::cout << std::endl;

	return false;
}

void pars(std::string line, std::vector<Hand> &hands)
{
	std::string cards;
	int value;

	cards = line.substr(0, 5);
	line.erase(0, 6);

	value = std::stoi(line);

	hands.push_back(Hand(cards, value));
}

void orderAllHands(std::vector<Hand> &hands)
{
	for (size_t i = 0; i < hands.size(); i++)
	{
		for (size_t j = i + 1; j < hands.size(); j++)
		{
			if (winAgainst(hands[i], hands[j]) == true)
			{
				Hand tmp = hands[i];
				hands[i] = hands[j];
				hands[j] = tmp;
			}
		}
	}
}

void resolve(bool debug, long long &total, std::vector<Hand> &hands)
{
	orderAllHands(hands);

	for (size_t i = 0; i < hands.size(); i++)
	{
		total += hands[i].getValue() * (i + 1);

		if (debug == true)
		{
			std::cout << "hand  : \033[0;34m";
			std::cout << hands[i].getHand();
			std::cout << "\033[0m -> | \033[0;34m";
			std::map<char, int> power = hands[i].getPower();
			for (std::map<char, int>::iterator it = power.begin(); it != power.end(); it++)
				std::cout << it->first << "\033[0m,\033[0;34m" << it->second << "\033[0m | \033[0;34m";
			std::cout << "\033[0m, value : \033[0;34m";
			std::cout << hands[i].getValue();
			std::cout << "\033[0m, to multyply by : \033[0;34m";
			std::cout << i + 1;
			std::cout << "\033[0m, to obtain : \033[0;32m";
			std::cout << hands[i].getValue() * (i + 1);
			std::cout << "\033[0m";
			std::cout << std::endl;
		}
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
	long long total = 0;
	bool debug = false;

	std::vector<Hand> hands;

	if (argc == 3)
		debug = true;

	while(std::getline(input, line))
	{
		if (line != "")
			pars(line, hands);
	}

	resolve(debug, total, hands);

	std::cout << "the total winnings are : \033[1;33m";
	std::cout << total;
	std::cout << "\033[0m" << std::endl;
	return (0);
}