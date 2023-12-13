#include <string>
#include <fstream>
#include <iostream>
#include <vector>

class Report
{
	private:
		std::vector<long>				_value;
		std::vector<std::vector<long>>	_logicalSequences;
		long							_extrapolatedValue;

	public:
		Report(std::vector<long> value);
		~Report();

		std::vector<long>	getValue() const;
		long				getExtrapolatedValue() const;

		bool				logicalSequencesComputingIsOver();
		void				computeLogicalSequences();
		void				computeExtrapolatedValue();

};

Report::Report(std::vector<long> value): _value(value), _logicalSequences(), _extrapolatedValue(0)
{
	_logicalSequences.push_back(_value);
}

Report::~Report()
{

}

std::vector<long> Report::getValue() const
{
	return (_value);
}

long	Report::getExtrapolatedValue() const
{
	return (_extrapolatedValue);

}

bool	Report::logicalSequencesComputingIsOver()
{
	std::vector<long> lastLogicalSequence = _logicalSequences.back();
	for (std::vector<long>::iterator it = lastLogicalSequence.begin(); it != lastLogicalSequence.end(); it++)
	{
		if (*it != 0)
			return (false);
	}
	return (true);
}

void	Report::computeLogicalSequences()
{
	while (this->logicalSequencesComputingIsOver() == false)
	{
		std::vector<long> lastLogicalSequence = _logicalSequences.back(), actualLogicalSequence;
		for (std::vector<long>::iterator it = lastLogicalSequence.begin(); it != lastLogicalSequence.end(); it++)
		{
			if (it + 1 != lastLogicalSequence.end())
				actualLogicalSequence.push_back(*(it + 1) - *it);
		}
		_logicalSequences.push_back(actualLogicalSequence);
	}
}

void	Report::computeExtrapolatedValue()
{
	long newValue = 0;

	for (size_t i = this->_logicalSequences.size() - 2; i > 0; i--)
	{
		newValue += this->_logicalSequences[i].back();
		this->_logicalSequences[i].push_back(newValue);
	}

	newValue += this->_logicalSequences[0].back();
	this->_logicalSequences[0].push_back(newValue);
	this->_extrapolatedValue = newValue;
}

void resolve(std::vector<Report> reports, long &total, bool debug)
{
	for (size_t i = 0; i < reports.size(); i++)
	{
		reports[i].computeLogicalSequences();
		reports[i].computeExtrapolatedValue();
		total += reports[i].getExtrapolatedValue();

		if (debug == true)
		{
			std::cout << "\033[0mThe Report number \033[0;34m";
			std::cout << i + 1;
			std::cout << "\033[0m with the value : \033[0;34m";
			std::vector<long> value = reports[i].getValue();
			for (std::vector<long>::iterator it = value.begin(); it != value.end(); it++)
			{
				std::cout << *it;
				std::cout << "\033[0m, \033[0;34m";
			}
			std::cout << "\033[0mhave a predicted extrapolated value of : \033[0;32m";
			std::cout << reports[i].getExtrapolatedValue();
			std::cout << "\033[0m for the next reading !";
			std::cout << std::endl;
		}
	}
}

void pars(std::string line, std::vector<Report> &reports)
{
	std::vector<long> value;

	while (line.size() > 0)
	{
		size_t posSpace = line.find_first_of(' ');
		if (posSpace == std::string::npos)
			posSpace = line.size();
	
		value.push_back(stoi(line.substr(0, posSpace)));
		line.erase(0, posSpace + 1);
	}

	reports.push_back(Report(value));
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
	std::vector<Report> reports;
	long total = 0;
	bool debug = false;

	if (argc == 3)
		debug = true;

	while(std::getline(input, line))
		pars(line, reports);

	resolve(reports, total, debug);

	std::cout << "The some of all extrapolated value is : \033[1;33m";
	std::cout << total;
	std::cout << "\033[0m" << std::endl;
	return (0);
}