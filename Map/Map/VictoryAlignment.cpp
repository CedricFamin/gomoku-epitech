#include <iostream>

#include "VictoryAlignment.h"


VictoryAlignment::VictoryAlignment(void)
{
}


VictoryAlignment::~VictoryAlignment(void)
{
}

void VictoryAlignment::enable()
{
	this->_enable = true;
}

void VictoryAlignment::disable()
{
	this->_enable = false;
}

bool VictoryAlignment::isEnable() const
{
	return this->_enable;
}

void print_case(unsigned long long int current);

bool VictoryAlignment::execute(Referrer & r, Goban::PION_TYPE pion, unsigned int x, unsigned int y)
{
	Goban::Case cCase = r.getGoban().GetMap()[y][x] >> 8;
	unsigned long long suitedMask[5] = 
	{
		pion | ((0x0 | (Goban::SUITED4 << Goban::SAFESIZE)) << Goban::COLORSIZE),
		pion | ((0x0 | (Goban::SUITED3 << Goban::SAFESIZE)) << Goban::COLORSIZE),
		pion | ((0x0 | (Goban::SUITED2 << Goban::SAFESIZE)) << Goban::COLORSIZE),
		pion | ((0x0 | (Goban::SUITED1 << Goban::SAFESIZE)) << Goban::COLORSIZE),
		0
	};

	for (int dir = 0; dir < 4; dir++)
	{
		unsigned long long int reverse_case = cCase >> Goban::PATTERNSIZE * 4;
		for (unsigned int i = 0, j = 4; i < 5; ++i, --j)
		{
			if ((cCase & Goban::PATTERNMASK & suitedMask[i]) == suitedMask[i])
			{
				if ((reverse_case & Goban::PATTERNMASK & suitedMask[j]) == suitedMask[j])
				{
					r.setWinner(pion);
					r.setGameFinished(true);
				}
			}
		}
		cCase >>= 7;
	}
	return false;
}


