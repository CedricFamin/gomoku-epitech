#include <functional>
#include <iostream>

#include "TakingRules.h"
#include "PatternIdentifier.h"

using namespace Rules;

TakingRules::TakingRules(void) : _enable(true)
{
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

bool TakingRules::execute(Goban & g, Goban::PION_TYPE pion, unsigned int x, unsigned int y)
{
	const int direction[8][2] = {
		{ 0,-1}, { 1, -1}, { 1, 0}, { 1, 1},
        { 0, 1}, {-1,  1}, {-1, 0}, {-1,-1}
	};

    Goban::Case cCase = g[y][x] >> Goban::HEADERSIZE;
	for (int i = 0; i < 8; ++i)
	{
        unsigned int lx = x + direction[i][0];
        unsigned int ly = y + direction[i][1];
        if ((cCase & Goban::PATTERNMASK) == Patterns::oox &&
            (g[ly][lx] & Goban::PIONMASK) != pion)
		{
			g.subIn(x + direction[i][0], y + direction[i][1]);
			g.subIn(x + direction[i][0] * 2, y + direction[i][1] * 2);
            g.Turns().back().captures.push_back(std::make_pair(x + direction[i][0], y + direction[i][1]));
            g.Turns().back().captures.push_back(std::make_pair(x + direction[i][0] * 2, y + direction[i][1] * 2));
		}
		cCase >>= Goban::PATTERNSIZE;
	}

	return true;
}
