#include <iostream>
#include <algorithm>

#include "VictoryAlignment.h"

using namespace Rules;

VictoryAlignment::VictoryAlignment(void) : _enable(true), _enableOptionalRule(true)
{
}


VictoryAlignment::~VictoryAlignment(void)
{
}

void VictoryAlignment::enableOptionalRule()
{
	this->_enableOptionalRule = true;
}

void VictoryAlignment::disableOptionalRule()
{
	this->_enableOptionalRule = false;
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

bool secure(Goban::Case const * const *map, unsigned int x, unsigned int y, int dir, int pion, bool && breaked = false)
{
	int move[4][2] = {
		{0,-1}, {1,-1}, {1,0}, {1,1}
	};
	int vsPion = (pion==Goban::RED) ? Goban::BLACK : Goban::RED;
	Goban::Case currentCase;
	unsigned long long int capture[2][2] =
	{
		{
			vsPion | Pattern<0,1,0x1>::value | (pion | Pattern<1,1,0x1>::value) << (Goban::PATTERNSIZE * 4), 
			0x3    | Pattern<0,1,0x1>::value | (0x3  | Pattern<1,1,0x3>::value) << (Goban::PATTERNSIZE * 4)
		},
		{
			pion | Pattern<1,1,0x1>::value | (vsPion | Pattern<0,1,0x1>::value) << (Goban::PATTERNSIZE * 4),
			0x3  | Pattern<1,1,0x3>::value | (0x3    | Pattern<0,1,0x1>::value) << (Goban::PATTERNSIZE * 4)
		}
	};
	for (int i = 0; i < 5; ++i)
	{
		x += move[dir][0];
		y += move[dir][1];
		currentCase = map[y][x];
		if ((currentCase & pion) != pion)
		{
			breaked = true;
			return false;
		}
		currentCase >>= Goban::HEADERSIZE;
		for (int j = 0; j < 8; ++j)
		{
			if ((currentCase & capture[0][1]) == capture[0][0])
				return false;
			if ((currentCase & capture[1][1]) == capture[1][0])
				return false;
			currentCase >>= Goban::PATTERNSIZE;
		}
	}
	return true;
}

bool VictoryAlignment::checkAlign(Referrer & r)
{
	bool victory = false;

	std::remove_if(this->_aligments.begin(), this->_aligments.end(),
		[this, &r, &victory](Align & align)->bool
	{
		bool breaked = false;
		if (!victory)
		{
			Goban::Case const * const * map = r.getGoban().GetMap();
			victory = secure(map, align.x, align.y, align.dir, align.pion, std::forward<bool>(breaked));
			if (victory == true)
			{
				r.setWinner(align.pion);
				r.setGameFinished(true);
			}
		}
		return breaked;
	});
	return victory;
}

bool VictoryAlignment::execute(Referrer & r, Goban::PION_TYPE pion, unsigned int x, unsigned int y)
{
	Goban::Case cCase = r.getGoban().GetMap()[y][x] >> 8;
	int move[4][2] = {{0,1}, {-1,1}, {-1,0}, {-1,-1}};
	unsigned long long int suitedMask[5] = 
	{
		pion | ((0x0 | (Goban::SUITED4 << Goban::SAFESIZE)) << Goban::COLORSIZE),
		pion | ((0x0 | (Goban::SUITED3 << Goban::SAFESIZE)) << Goban::COLORSIZE),
		pion | ((0x0 | (Goban::SUITED2 << Goban::SAFESIZE)) << Goban::COLORSIZE),
		pion | ((0x0 | (Goban::SUITED1 << Goban::SAFESIZE)) << Goban::COLORSIZE),
		0
	};

	if (this->checkAlign(r) == true)
		return true;
	for (int dir = 0; dir < 4; dir++)
	{
		unsigned long long int reverse_case = cCase >> Goban::PATTERNSIZE * 4;
		for (unsigned int i = 0, j = 4; i < 5; ++i, --j)
		{
			if ((cCase & Goban::PATTERNMASK & suitedMask[i]) == suitedMask[i] &&
				(reverse_case & Goban::PATTERNMASK & suitedMask[j]) == suitedMask[j])
			{
				if (!this->_enableOptionalRule || secure(r.getGoban().GetMap(), move[dir][0] * i + x, move[dir][1] * i + y, dir, pion))
				{
					r.setWinner(pion);
					r.setGameFinished(true);
				}
				else
				{
					Align align = {move[dir][0] * i + x, move[dir][1] * i + y, dir, pion};
					this->_aligments.push_back(align);
				}
			}
		}
		cCase >>= 7;
	}
	return false;
}


