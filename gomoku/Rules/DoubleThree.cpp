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

int isFree3(Patterns::PatternInfos & p1, Patterns::PatternInfos & p2, Goban & g, Goban::PION_TYPE p, unsigned int x, unsigned int y, int dir, bool extand = true);

bool isPartOfFree3(Goban &g, unsigned int x, unsigned int y, Goban::PION_TYPE p, int index, int dir)
{
	x = GobanIterator::direction[dir][0] * index;
	y = GobanIterator::direction[dir][1] * index;

	Goban::Case toEval = g[y][x] >> Goban::HEADERSIZE;
	Patterns::PatternInfos p1, p2;
	for (int i = 0; i < 4; ++i)
	{
		if (i != dir)
		{
			p1 = Patterns::GetPatternInfos(toEval & Goban::PATTERNMASK);
			p2 = Patterns::GetPatternInfos((toEval >> Goban::PATTERNSIZE * 4) & Goban::PATTERNMASK);
			if (isFree3(p1, p2, g, p, x, y, i, false)) return true;
		}
	}
	return false;
}

int isFree3(Patterns::PatternInfos & p1, Patterns::PatternInfos & p2, Goban & g, Goban::PION_TYPE p, unsigned int x, unsigned int y, int dir, bool extand)
{
	const int direction[4][2] = {
		{ 0,-1}, { 1, -1}, { 1, 0}, { 1, 1}
	};
	Goban::Case pion1 = g[y + direction[dir][1] * p1.caseIndex][x + direction[dir][0] * p1.caseIndex] & Goban::PIONMASK;
	Goban::Case pion2 = g[y - direction[dir][1] * p2.caseIndex][x - direction[dir][0] * p2.caseIndex] & Goban::PIONMASK;

	if (pion1 == p && pion2 == p && p1.free && p2.free)
	{
		if (p1.align == 1 && p2.align == 1) return 1;
		if (p1.align == 1 && p2.caseIndex == 2 && p2.expand >= 2 && p2.pattern != Patterns::_oo_) return 1;
		if (p2.align == 1 && p1.caseIndex == 2 && p1.expand >= 2 && p1.pattern != Patterns::_oo_) return 1;
	}
	if (pion1 == p && p1.free && (p2.pattern == 0 || p2.caseIndex > 1))
	{
		if (p1.align == 2 ||  p1.expand == 2) 
			return 1;
	}
	if (pion2 == p && p2.free && (p1.pattern == 0 || p1.caseIndex > 1))
	{
		if (p2.align == 2) return 1;
		if (p2.expand == 2) return 1;
	}
	return 0;
}

bool DoubleThree::execute(Goban & g, Goban::Turn & turn)
{
    
	Patterns::PatternInfos pInfos1, pInfos2;
    Goban::Case cCase = g[turn.y][turn.x] >> Goban::HEADERSIZE;
	int double3 = 0;

	for (int i = 0; i < 4; ++i)
	{
        pInfos1 = Patterns::GetPatternInfos(cCase & Goban::PATTERNMASK);
		pInfos2 = Patterns::GetPatternInfos((cCase >> (Goban::PATTERNSIZE * 4)) & Goban::PATTERNMASK);
		double3 += isFree3(pInfos1, pInfos2, g, turn.pion, turn.x, turn.y , i);
		if (double3 > 1) return false;
		cCase >>= Goban::PATTERNSIZE;
	}
    return true;
}
