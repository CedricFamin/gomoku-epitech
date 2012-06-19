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

bool Referrer::operator()(Goban & g, Goban::PION_TYPE pion, unsigned int x, unsigned int y)
{
	bool canPlay = true;
	bool finished = false;

	if (g.gameFinished()) return false;
	if (!g.InBound(x, y)) return false;
	std::for_each(this->_prePlayRules.begin(), this->_prePlayRules.end(),
	[&g, pion, x, y, &canPlay](Rules::IRule * rule)
	{
		if (rule->isEnable() && canPlay)
			if (!rule->execute(g, pion, x, y))
				canPlay = false;
	});
	if (!canPlay) return false;

	g.Turns().push_back(Goban::Turn(pion, x, y));
	g.Putin(pion, x, y);
	std::for_each(this->_playRules.begin(), this->_playRules.end(),
	[&g, pion, x, y](Rules::IRule * rule)
	{
		if (rule->isEnable())
			rule->execute(g, pion, x, y);
	});

	std::for_each(this->_postPlayRules.begin(), this->_postPlayRules.end(),
	[&g, &finished, pion, x, y](Rules::IRule * rule)
	{
		if (rule->isEnable()) finished |= rule->execute(g, pion, x, y);
	});
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
