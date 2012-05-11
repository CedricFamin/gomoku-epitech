#include "VictoryCapturesRule.h"


VictoryCapturesRule::VictoryCapturesRule(TakingRules const & rule) : _takingRule(rule), _enable(true)
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

std::string const & VictoryCapturesRule::name() const
{
	return "VictoryCaptures";
}

bool VictoryCapturesRule::execute(Referrer & r, Goban::PION_TYPE pion, unsigned int x, unsigned int y)
{
	if (this->_takingRule.getCaptures()[Goban::RED >> 1] >= 10 ||
		this->_takingRule.getCaptures()[Goban::BLACK >> 1] >= 10)
	{
		if (this->_takingRule.getCaptures()[Goban::RED >> 1] >= 10)
			r.setGameFinished(Goban::RED);
		else
			r.setGameFinished(Goban::BLACK);
		r.setWinner(pion);
		return true;
	}
	return false;
}


