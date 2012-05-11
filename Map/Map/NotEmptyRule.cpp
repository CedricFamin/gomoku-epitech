#include "NotEmptyRule.h"


NotEmptyRule::NotEmptyRule(void)
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

std::string const & NotEmptyRule::name() const
{
	return "NotEmpty";
}

bool NotEmptyRule::execute(Referrer & r, Goban::PION_TYPE pion, unsigned int x, unsigned int y)
{
	return r.getGoban().GetMap()[y][x].color == 0;
}