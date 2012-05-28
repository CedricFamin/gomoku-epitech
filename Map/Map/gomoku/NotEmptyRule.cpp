#include "NotEmptyRule.h"

using namespace Rules;

NotEmptyRule::NotEmptyRule(void) : _enable(true)
{
}


NotEmptyRule::~NotEmptyRule(void)
{
}

void NotEmptyRule::enable()
{
	this->_enable = true;
}

void NotEmptyRule::disable()
{
	this->_enable = false;
}

bool NotEmptyRule::isEnable() const
{
	return this->_enable;
}

bool NotEmptyRule::execute(Referrer & r, Goban::PION_TYPE, unsigned int x, unsigned int y)
{
    return ((r.getGoban().GetMap()[y][x] & Goban::PIONMASK) == 0);
}
