#include "EachInTurnRule.h"

using namespace Rules;

EachInTurnRule::EachInTurnRule(void) : _enable(true)
{
}


EachInTurnRule::~EachInTurnRule(void)
{
}

void EachInTurnRule::enable()
{
	this->_enable = true;
}

void EachInTurnRule::disable()
{
	this->_enable = false;
}

bool EachInTurnRule::isEnable() const
{
	return this->_enable;
}

bool EachInTurnRule::execute(Referrer & r, Goban::PION_TYPE pion, unsigned int, unsigned int)
{
	return r.GetListOfTurn().size() == 0 || r.GetListOfTurn().back().pion != pion;
}