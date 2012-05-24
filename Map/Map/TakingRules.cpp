#include <functional>
#include <iostream>

#include "TakingRules.h"

using namespace Rules;

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

int const * TakingRules::getCaptures() const
{
	return this->_capture;
}

bool TakingRules::execute(Referrer & r, Goban::PION_TYPE pion, unsigned int x, unsigned int y)
{
	const int direction[8][2] = {
		{ 0,-1}, { 1, -1}, { 1, 0}, { 1, 1},
		{0, 1}, {-1, 1}, { -1,0}, { -1,-1}
	};

	Goban::Case cCase = r.getGoban().GetMap()[y][x] >> Goban::HEADERSIZE;
	unsigned long long int capture, mask = Goban::PIONMASK | Pattern<0,3, 0x3>::mask;
	
	capture = ((pion==Goban::RED) ? Goban::BLACK : Goban::RED) | Pattern<0,3, 0x3>::value;

	
	for (int i = 0; i < 8; ++i)
	{
		if ((cCase & mask) == capture)
		{
			r.getGoban().subIn(x + direction[i][0], y + direction[i][1]);
			r.getGoban().subIn(x + direction[i][0] * 2, y + direction[i][1] * 2);
			this->_capture[pion >> 1] += 2;
		}
		cCase >>= Goban::PATTERNSIZE;
	}

	return true;
}