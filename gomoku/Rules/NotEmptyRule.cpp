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

bool NotEmptyRule::execute(Goban & g, Goban::Turn & turn)
{
    return ((g[turn.y][turn.x] & Goban::PIONMASK) == 0);
}
