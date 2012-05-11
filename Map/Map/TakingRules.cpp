#include <functional>
#include <iostream>

#include "TakingRules.h"

TakingRules::TakingRules(void)
{
	_capture[0] = 0;
	_capture[1] = 0;
}


TakingRules::~TakingRules(void)
{
}

void TakingRules::enable()
{
	this->_enable = true;
}

void TakingRules::disable()
{
	this->_enable = false;
}

bool TakingRules::isEnable() const
{
	return this->_enable;
}

std::string const & TakingRules::name() const
{
	return "NotEmpty";
}

int const * TakingRules::getCaptures() const
{
	return this->_capture;
}

bool TakingRules::execute(Referrer & r, Goban::PION_TYPE pion, unsigned int x, unsigned int y)
{
	int origin[8][2] = {
		{ 0,-1}, { 1,-1}, { 1, 0}, { 1, 1},
		{ 0, 1}, {-1, 1}, {-1, 0}, {-1,-1},
	};

	TURN_AROUND(r.getGoban().GetMap(), x, y, r.getGoban().getWidth(), r.getGoban().getHeight())
	{
		if (currentElem == 0)
			continue;
		if (currentElem->color && currentElem->pair.rawData && currentElem->color != pion)
		{
			if (EXTRACTBIT((currentElem->pair.rawData ^ currentElem->pair_safe.rawData), TURN_INDEX))
			{
				r.getGoban().subIn(TURN_X, TURN_Y);
				r.getGoban().subIn(TURN_X + (origin[TURN_INDEX][0]), TURN_Y + (origin[TURN_INDEX][1]));
				this->_capture[pion >> 1] += 2;
			}
		}
	}
	return true;
}