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

bool VictoryCapturesRule::execute(Goban & g, Goban::PION_TYPE, unsigned int, unsigned int)
{
	if (g.deletedStone(Goban::BLACK) >= 10 ||
		g.deletedStone(Goban::RED) >= 10)
	{
		if (g.deletedStone(Goban::RED) >= 10)
			g.setWinner(Goban::RED);
		else
			g.setWinner(Goban::BLACK);
		g.setGameFinished(true);
		return true;
	}
	return false;
}
