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

std::string const & VictoryAlignment::name() const
{
	return "VictoryCaptures";
}

int countAlign(Goban & g, unsigned int x, unsigned int y, unsigned char direction, Goban::PION_TYPE p, bool & safe)
{
	Goban::Case & currentCase = g.GetMap()[y][x];
	int dir[8][2] = {
		{0,-1}, {1,-1}, {1,0}, {1,1},
		{0,1}, {-1,1}, {-1,0}, {-1,-1},
	};

	if (x >= g.getWidth() || y >= g.getHeight() || !currentCase.color ||
		GETPIONTYPE(currentCase) != p)
		return 0;
	if (currentCase.pair_safe.rawData ^ currentCase.pair.rawData)
		safe = false;
	if ((currentCase.pair.rawData & (1 << (direction))))
		return 1 + countAlign(g, x + dir[direction][0], y + dir[direction][1], direction, p, safe);
	return 1;
}

bool VictoryAlignment::execute(Referrer & r, Goban::PION_TYPE pion, unsigned int x, unsigned int y)
{
	int align;
	char direction = 0x0;
	bool safe = true;

	Goban::Case ** map = r.getGoban().GetMap();
	
	while (direction < 4)
	{
		align = 0;
		align = countAlign(r.getGoban(), x, y, direction, pion, safe);
		align += countAlign(r.getGoban(), x, y, 4+(direction), pion, safe) - 1;
		if (align >= 5)
		{
			if(safe)
				r.setGameFinished(true);
			r.setWinner(pion);
			return true;
		}
		++direction;
	}
	return false;
}


