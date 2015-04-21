#include <vector>
#include <string>

class Parser
{
public:
	Parser();
	~Parser()
	{
		;
	}

	void Number2Words(long long int num);
	void Number2WordsSmall(long long int num);

	std::string GetResultStr()
	{
		return m_Words;
	}

private:
	Parser& operator=(const Parser&);
	Parser(const Parser&);

	bool m_bUseAnd;
	std::string m_Words;
	//std::vector<std::string> m_MatchOrders;
	std::vector<std::pair<long long int, std::string>> m_MatchOrders;
	std::vector<std::pair<long long int, std::string>> m_MatchOrdersSmall;
	std::vector<std::string> m_MatchUnits;
	std::vector<std::string> m_MatchTens;
};