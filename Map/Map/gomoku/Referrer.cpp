#include <algorithm>

#include "Referrer.h"


Referrer::Referrer(Goban & goban) : _goban(&goban), _gameFinished(false)
{

}


Referrer::~Referrer(void)
{
}

bool Referrer::CanPlay(Goban::PION_TYPE pion, int x, int y)
{
	bool canPlay = true;
	
	if (this->_gameFinished)
		return false;
    if (!_goban->InBound(x, y))
        return false;
	std::for_each(this->_prePlayRules.begin(), this->_prePlayRules.end(),
		[this, pion, x, y, &canPlay](Rules::IRule * rule)
	{
		if (rule->isEnable() && canPlay)
			if (!rule->execute(*this, pion, x, y))
				canPlay = false;
	});
	this->_currentTurn.pion = pion;
	this->_currentTurn.x = x;
	this->_currentTurn.y = y;
	this->_currentTurn.validate = true;
	if (canPlay == false)
		this->_currentTurn.validate = false;
	return this->_currentTurn.validate;
}

bool Referrer::Play()
{
	if (this->_currentTurn.validate == false)
		return false;
	this->_currentTurn.validate = false;
	this->_playedTurns.push_back(this->_currentTurn);
    this->_goban->Putin(this->_currentTurn.pion,
		this->_currentTurn.x,
		this->_currentTurn.y);
	std::for_each(this->_playRules.begin(), this->_playRules.end(),
		[this](Rules::IRule * rule)
	{
		if (rule->isEnable())
			rule->execute(*this, this->_currentTurn.pion, this->_currentTurn.x, this->_currentTurn.y);
	});
	return true;
}

bool Referrer::AfterPlay()
{
	bool finished = false;

	std::for_each(this->_postPlayRules.begin(), this->_postPlayRules.end(),
		[this, &finished](Rules::IRule * rule)
	{
		if (rule->isEnable() && this->_playedTurns.size())
			finished |= rule->execute(*this, this->_playedTurns.back().pion, this->_playedTurns.back().x, this->_playedTurns.back().y);
	});
	return finished;
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

Goban & Referrer::getGoban() const
{
    return *this->_goban;
}

void Referrer::setGoban(Goban & g)
{
    this->_goban = &g;
}

std::list<Referrer::Turn> & Referrer::GetListOfTurn()
{
	return this->_playedTurns;
}

bool Referrer::GameFinished() const
{
	return this->_gameFinished;
}

void Referrer::setGameFinished(bool value)
{
	this->_gameFinished = value;
}

void Referrer::setWinner(Goban::PION_TYPE value)
{
	this->_winner = value;
}

Goban::PION_TYPE Referrer::Winner() const
{
	return this->_winner;
}
