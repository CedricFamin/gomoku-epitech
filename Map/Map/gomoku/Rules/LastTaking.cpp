#include "LastTaking.h"

using namespace Rules;

LastTaking::LastTaking(void) : _enable(true)
{
}


LastTaking::~LastTaking(void)
{
}

void LastTaking::enable()
{
	this->_enable = true;
}

void LastTaking::disable()
{
	this->_enable = false;
}

bool LastTaking::isEnable() const
{
	return this->_enable;
}

bool LastTaking::execute(Referrer &, Goban::PION_TYPE, unsigned int, unsigned int)
{
	return false;
}
