#include <algorithm>

#include "Referrer.h"


Referrer::Referrer()
{

}

Referrer::Referrer(Referrer const & r) : _prePlayRules(r._prePlayRules), _playRules(r._playRules), _postPlayRules(r._postPlayRules)
{
}


Referrer::~Referrer(void)
{
}

bool Referrer::operator()(Goban & g, Goban::PION_TYPE pion, unsigned int x, unsigned int y, bool useHistory)
{
	bool canPlay = true;
	bool finished = false;

	if (g.gameFinished()) return false;
	if (!g.InBound(x, y)) return false;
	{
		auto begin = this->_prePlayRules.begin(), end = this->_prePlayRules.end();
		for(;begin != end;++begin)
		{
			if ((*begin)->isEnable() && canPlay)
				if (!(*begin)->execute(g, pion, x, y))
					canPlay = false;
		}
	}
	if (!canPlay) return false;
	if (useHistory) g.Turns().push_back(Goban::Turn(pion, x, y));
	g.Putin(pion, x, y);
	{
		auto begin = this->_playRules.begin(), end = this->_playRules.end();
		for(;begin != end;++begin)
			if ((*begin)->isEnable()) (*begin)->execute(g, pion, x, y);
	}
	{
		auto begin = this->_postPlayRules.begin(), end = this->_postPlayRules.end();
		for(;begin != end;++begin)
			if ((*begin)->isEnable()) finished |= (*begin)->execute(g, pion, x, y);
	}
	return true;
}

void Referrer::addPrePlayRule(Rules::IRule & rule)
{
	this->_prePlayRules.push_back(&rule);
}

void Referrer::addPlayRule(Rules::IRule & rule)
{
	this->_playRules.push_back(&rule);
}

void Referrer::addPostPlayRule(Rules::IRule & rule)
{
	this->_postPlayRules.push_back(&rule);
}
