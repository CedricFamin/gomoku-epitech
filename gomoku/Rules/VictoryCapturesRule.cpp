#include "VictoryCapturesRule.h"

using namespace Rules;

VictoryCapturesRule::VictoryCapturesRule() : _enable(true)
{
}


VictoryCapturesRule::~VictoryCapturesRule(void)
{
}

void VictoryCapturesRule::enable()
{
	this->_enable = true;
}

void VictoryCapturesRule::disable()
{
	this->_enable = false;
}

bool VictoryCapturesRule::isEnable() const
{
	return this->_enable;
}

bool VictoryCapturesRule::execute(Referrer & r, Goban::PION_TYPE, unsigned int, unsigned int)
{
	if (r.getGoban().deletedStone(Goban::BLACK) >= 10 ||
		r.getGoban().deletedStone(Goban::RED) >= 10)
	{
		if (r.getGoban().deletedStone(Goban::RED) >= 10)
			r.setWinner(Goban::RED);
		else
			r.setWinner(Goban::BLACK);
		r.setGameFinished(true);
		return true;
	}
	return false;
}
