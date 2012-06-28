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

bool TakingRules::execute(Goban & g, Goban::Turn & turn)
{
	const int direction[8][2] = {
		{ 0,-1}, { 1, -1}, { 1, 0}, { 1, 1},
        { 0, 1}, {-1,  1}, {-1, 0}, {-1,-1}
	};

    Goban::Case cCase = g[turn.y][turn.x] >> Goban::HEADERSIZE;
	for (int i = 0; i < 8; ++i)
	{
        unsigned int lx = turn.x + direction[i][0];
        unsigned int ly = turn.y + direction[i][1];
        if ((cCase & Goban::PATTERNMASK) == Patterns::oox &&
            (g[ly][lx] & Goban::PIONMASK) != turn.pion)
		{
			g.subIn(turn.x + direction[i][0], turn.y + direction[i][1]);
			g.subIn(turn.x + direction[i][0] * 2, turn.y + direction[i][1] * 2);
			g.toDelete.push(std::make_pair(turn.x + direction[i][0], turn.y + direction[i][1]));
			g.toDelete.push(std::make_pair(turn.x + direction[i][0] * 2, turn.y + direction[i][1] * 2));
		}
		cCase >>= Goban::PATTERNSIZE;
	}

	return true;
}
