#include <algorithm>

#include "Referrer.h"


Referrer::Referrer()
{

}

Referrer::Referrer(Referrer const & r) : _prePlayRules(r._prePlayRules), _playRules(r._playRules), _postPlayRules(r._postPlayRules)
{
	this->_beginPrePlay = this->_prePlayRules.begin();
	this->_endPrePlay = this->_prePlayRules.end();
	this->_beginPlay = this->_playRules.begin();
	this->_endPlay = this->_playRules.end();
	this->_beginPostPlay = this->_postPlayRules.begin();
	this->_endPostPlay = this->_postPlayRules.end();
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
	Goban::Turn turn = Goban::Turn(pion, x, y);
	for(auto begin = this->_beginPrePlay;begin != this->_endPrePlay;++begin)
	{
		if ((*begin)->isEnable() && canPlay)
			if (!(*begin)->execute(g, turn))
				canPlay = false;
	}
	if (!canPlay) return false;
	g.Putin(pion, x, y);
	for(auto begin = this->_beginPlay;begin != this->_endPlay;++begin)
		if ((*begin)->isEnable()) (*begin)->execute(g, turn);
	for(auto begin = this->_beginPostPlay;begin != this->_endPostPlay;++begin)
		if ((*begin)->isEnable()) finished |= (*begin)->execute(g, turn);
	if (finished) g.setGameFinished(true);
	if (useHistory) g.Turns().push_back(turn);
	return true;
}

void Referrer::addPrePlayRule(Rules::IRule & rule)
{
	this->_prePlayRules.push_back(&rule);
	this->_beginPrePlay = this->_prePlayRules.begin();
	this->_endPrePlay = this->_prePlayRules.end();
}

void Referrer::addPlayRule(Rules::IRule & rule)
{
	this->_playRules.push_back(&rule);
	this->_beginPlay = this->_playRules.begin();
	this->_endPlay = this->_playRules.end();
}

void Referrer::addPostPlayRule(Rules::IRule & rule)
{
	this->_postPlayRules.push_back(&rule);
	this->_beginPostPlay = this->_postPlayRules.begin();
	this->_endPostPlay = this->_postPlayRules.end();

}
