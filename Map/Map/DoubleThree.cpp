#include "Referrer.h"
#include "Goban.h"
#include "DoubleThree.h"

DoubleThree::DoubleThree(void)
{
}


DoubleThree::~DoubleThree(void)
{
}

void DoubleThree::enable()
{
	this->_enable = true;
}

void DoubleThree::disable()
{
	this->_enable = false;
}

bool DoubleThree::isEnable() const
{
	return this->_enable;
}

std::string const & DoubleThree::name() const
{
	return "EachInTurn";
}

bool DoubleThree::execute(Referrer & r, Goban::PION_TYPE pion, unsigned int x, unsigned int y)
{
	int dir[8][2] = {
		{0,-2}, {2,-2}, {2,0}, {2,2},
		{0,2}, {-2,2}, {-2,0}, {-2,-2},
	};
	Goban::Case ** map = r.getGoban().GetMap();
	int doublethree = 0;
	char doubleThreeDir = 0;

	TURN_AROUND(map, x, y, r.getGoban().getWidth(), r.getGoban().getHeight())
	{
		unsigned int lx = x + dir[TURN_INDEX][0];
		unsigned int ly = y + dir[TURN_INDEX][1];
		unsigned int ldx = TURN_X + dir[TURN_INDEX][0];
		unsigned int ldy = TURN_Y + dir[TURN_INDEX][1];
		if (currentElem &&
			!EXTRACTBIT(doubleThreeDir, TURN_INDEX) &&
			EXTRACTBIT(map[y][x].free_case.rawData, (4 + TURN_INDEX) % 8) &&

			(GETPIONTYPE((*currentElem)) == pion &&
			EXTRACTBIT(currentElem->pair.rawData, TURN_INDEX) &&
			EXTRACTBIT(currentElem->pair_safe.rawData, TURN_INDEX)) ||

			(r.getGoban().InBound(lx, ly) &&
			GETPIONTYPE(map[ly][lx]) == pion &&
			EXTRACTBIT(map[ly][lx].pair.rawData, TURN_INDEX) &&
			EXTRACTBIT(map[ly][lx].pair_safe.rawData, TURN_INDEX))
			
			/*(GETPIONTYPE((*currentElem)) == pion &&
			EXTRACTBIT(currentElem->free_case.rawData, TURN_INDEX) &&
			r.getGoban().InBound(ldx, ldy) &&
			map[ldy][ldx].color == pion &&
			EXTRACTBIT(map[ldy][ldx].free_case.rawData, TURN_INDEX)*/)
		{
			doubleThreeDir |= 1 << ((4 + TURN_INDEX) % 8);
			doublethree++;
		}
	}
	return doublethree <= 1;
}