#include "LastTaking.h"


LastTaking::LastTaking(void)
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

std::string const & LastTaking::name() const
{
	return "NotEmpty";
}

bool LastTaking::execute(Referrer & r, Goban::PION_TYPE pion, unsigned int x, unsigned int y)
{
	return false;
}