#pragma once

#include"Parser.h"
#include <string>
#include <cmath>
#include <vector>
#include <cstdint>

using namespace std;

Parser::Parser()
:m_Words("")
{
	//Korean
	m_bUseAnd = false;
#define INSERT_TO_MATCH_ORDER(i, str) m_MatchOrders.push_back(make_pair(i, str))	
	INSERT_TO_MATCH_ORDER(1000000000000, " jo, ");
	INSERT_TO_MATCH_ORDER(100000000, " uk, ");
	INSERT_TO_MATCH_ORDER(10000, " man, ");
#undef INSERT_TO_MATCH_ORDER

#define INSERT_TO_MATCH_ORDER_SMALL(i, str) m_MatchOrdersSmall.push_back(make_pair(i, str))
	INSERT_TO_MATCH_ORDER_SMALL(1000, " cheon ");
	INSERT_TO_MATCH_ORDER_SMALL(100, "  bak ");
	INSERT_TO_MATCH_ORDER_SMALL(10, " sib ");
#undef INSERT_TO_MATCH_ORDER_SMALL

	m_MatchUnits = { "bbang", "il", "ee", "sam", "sa", "o", "yuk", "chil", "pal", "gu" };

//	//English
//	m_bUseAnd = true;
//#define INSERT_TO_MATCH_ORDER(i, str) m_MatchOrders.push_back(make_pair(i, str))
//	INSERT_TO_MATCH_ORDER(1000000000000, " trillion, ");
//	INSERT_TO_MATCH_ORDER(1000000000, " billion, ");
//	INSERT_TO_MATCH_ORDER(1000000, " million, ");
//	INSERT_TO_MATCH_ORDER(1000, " thousand, ");
//#undef INSERT_TO_MATCH_ORDER
//
//#define INSERT_TO_MATCH_ORDER_SMALL(i, str) m_MatchOrdersSmall.push_back(make_pair(i, str))
//	INSERT_TO_MATCH_ORDER_SMALL(100, " hundred ");
//#undef INSERT_TO_MATCH_ORDER_SMALL
//
//	m_MatchUnits = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen" };
//	m_MatchTens = { "zero", "ten", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};
}

void Parser::Number2Words(long long int numb)
{
	if (numb == 0)
	{
		m_Words = "zero";
		return;
	}

	if (numb < 0)
	{
		m_Words = "minus ";
		Number2Words(abs(numb));
		return;
	}

	for (auto it = m_MatchOrders.begin(); it != m_MatchOrders.end(); ++it)
	{
		long long int divisor = (*it).first;

		if ((numb / divisor) > 0)
		{
			Number2WordsSmall(numb / divisor);	// under 1000, for english

			m_Words += (*it).second;
			numb %= divisor;
		}
	}

	Number2WordsSmall(numb);	// under 100, for english
	
	return;
}

void Parser::Number2WordsSmall(long long int numb)
{
	for (auto it = m_MatchOrdersSmall.begin(); it != m_MatchOrdersSmall.end(); ++it)
	{
		long long int divisor = (*it).first;

		if ((numb / divisor) > 0)
		{
			Number2WordsSmall(numb / divisor);

			m_Words += (*it).second;
			numb %= divisor;
		}
	}
	
	if (numb > 0)	// number under 100
	{
		if (m_bUseAnd)
		{
			if (m_Words != "")
				m_Words += "and ";
		}
		
		if (numb < m_MatchUnits.size())
			m_Words += m_MatchUnits[numb];
		else
		{
			m_Words += m_MatchTens[numb / 10];
			if ((numb % 10) > 0)
				m_Words += "-" + m_MatchUnits[numb % 10];
		}
	}
}