#include "Referrer.h"
#include "Goban.h"
#include "DoubleThree.h"

#include "PatternIdentifier.h"
#include "GobanIterator.h"

using namespace Rules;

DoubleThree::DoubleThree(void) : _enable(true)
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

int isFree3(const Patterns::PatternInfos * p1, const Patterns::PatternInfos * p2, Goban & g, Goban::PION_TYPE p, unsigned int x, unsigned int y, int dir, bool extand = true);

bool isPartOfFree3(Goban &g, unsigned int x, unsigned int y, Goban::PION_TYPE p, int index, int dir)
{
	x += GobanIterator::direction[dir][0] * index;
	y += GobanIterator::direction[dir][1] * index;

	Goban::Case toEval = g[y][x] >> Goban::HEADERSIZE;
	const Patterns::PatternInfos * p1, * p2;
	for (int i = 0; i < 4; ++i)
	{
		if (i != dir)
		{
			p1 = Patterns::patterns + (toEval & Goban::PATTERNMASK);
			p2 = Patterns::patterns + ((toEval >> Goban::PATTERNSIZE * 4) & Goban::PATTERNMASK);
			if (isFree3(p1, p2, g, p, x, y, i, false)) return true;
		}
		toEval >>= Goban::PATTERNSIZE;
	}
	return false;
}

int isFree3(const Patterns::PatternInfos * p1, const Patterns::PatternInfos * p2, Goban & g, Goban::PION_TYPE p, unsigned int x, unsigned int y, int dir, bool extand)
{
	const int direction[4][2] = {
		{ 0,-1}, { 1, -1}, { 1, 0}, { 1, 1}
	};
	unsigned int x1 = x + direction[dir][0] * p1->caseIndex, x2 = x - direction[dir][0] * p2->caseIndex;
	unsigned int y1 = y + direction[dir][1] * p1->caseIndex, y2 = y - direction[dir][1] * p2->caseIndex;
	Goban::Case pion1 = g[y1][x1] & Goban::PIONMASK;
	Goban::Case pion2 = g[y2][x2] & Goban::PIONMASK;
	std::pair<int, int> coord(0, 0);

	if (pion1 == p && pion2 == p && p1->free && p2->free)
	{
		if (p1->align == 1 && p2->align == 1) coord = std::make_pair<int, int>(1, -1);
		else if (p1->align == 1 && p2->caseIndex == 2 && p2->expand >= 2 && p2->pattern != Patterns::_oo_) coord = std::make_pair<int, int>(1, -2);
		else if (p2->align == 1 && p1->caseIndex == 2 && p1->expand >= 2 && p1->pattern != Patterns::_oo_) coord = std::make_pair<int, int>(-1, 2);
	}
	if (pion1 == p && p1->free && (p1->size - p1->expand >= 2) && (p2->pattern == 0 || p2->caseIndex > 1))
	{

		if (p1->align == 2) coord = std::make_pair<int, int>(1, 2);
		else if (p1->align == 1 && p1->expand == 2) coord = std::make_pair<int, int>(1, 3);
		else if (p1->expand >= 2) coord = std::make_pair<int, int>(2, 3);
	}
	if (pion2 == p && p2->free && (p2->size - p2->expand >= 2) && (p1->pattern == 0 || p1->caseIndex > 1))
	{
		
		if (p2->align == 2) coord = std::make_pair<int, int>(-1, -2);
		else if (p2->align == 1 && p2->expand == 2) coord = std::make_pair<int, int>(-1, -3);
		else if (p2->expand >= 2) coord = std::make_pair<int, int>(-2, -3);
	}
	if (coord.first != 0)
	{
		if (extand)
		{
			if ((isPartOfFree3(g, x, y, p, coord.first, dir) || isPartOfFree3(g, x, y, p, coord.second, dir)))
				return 2;
		}
		return 1;
	}
	return 0;
}

bool DoubleThree::execute(Goban & g, Goban::Turn & turn)
{
    
	const Patterns::PatternInfos * pInfos1, * pInfos2;
    Goban::Case cCase = g[turn.y][turn.x] >> Goban::HEADERSIZE;
	int double3 = 0;

	for (int i = 0; i < 4; ++i)
	{
        pInfos1 = Patterns::patterns + (cCase & Goban::PATTERNMASK);
		pInfos2 = Patterns::patterns + ((cCase >> (Goban::PATTERNSIZE * 4)) & Goban::PATTERNMASK);
		double3 += isFree3(pInfos1, pInfos2, g, turn.pion, turn.x, turn.y , i);
		if (double3 > 1) return false;
		cCase >>= Goban::PATTERNSIZE;
	}
    return true;
}
