#include "Evaluator.h"
#include "GobanIterator.h"
#include "PatternIdentifier.h"


Evaluator::Evaluator(Goban const * g) : _base(g)
{
}


Evaluator::~Evaluator(void)
{
}

int Evaluator::operator()(Goban & g, Goban::PION_TYPE p)
{   
	Goban::PION_TYPE currentPion;
    unsigned long long int current;
	Goban::PION_TYPE other = Goban::Other(p);
	int score = 0;
	int captures = g.deletedStone(other) - this->_base->deletedStone(p);
	int value;
	typedef int (Evaluator::*ScoringMethod)(Goban &, unsigned int, unsigned int, int, Goban::PION_TYPE);
	ScoringMethod scorings[] = {
		&Evaluator::_emptyPattern,
		&Evaluator::__o_Pattern,
		&Evaluator::__ooPattern,
		&Evaluator::__oo_Pattern,
		&Evaluator::__ooxPattern,
		&Evaluator::__oooPattern,
		&Evaluator::_o_Pattern,
		&Evaluator::_o_o_Pattern,
		&Evaluator::_oxPattern,
		&Evaluator::_ooPattern,
		&Evaluator::_oo_Pattern,
		&Evaluator::_ooxPattern,
		&Evaluator::_oooPattern,
		&Evaluator::_ooo_Pattern,
		&Evaluator::_oooxPattern,
		&Evaluator::_ooooPattern,
	};
	for (unsigned int y = 0; y < 19; ++y)
	{
		for (unsigned int x = 0; x < 19; ++x)
		{
			if (!g.InBound(x, y))
				continue;
			current = g[y][x];
			value = 0;
			currentPion = (Goban::PION_TYPE)(current & Goban::PIONMASK);
			current >>= Goban::HEADERSIZE;
			if (currentPion == 0 && current)
				for (unsigned int d = 0; d < 8; ++d, current >>= Goban::PATTERNSIZE)
				{
					int pattern = (current & Goban::PATTERNMASK);
					if (pattern == Patterns::oox || pattern == Patterns::ox)
						value += (this->*scorings[current & Goban::PATTERNMASK])(g, x, y, d, p);
				}
			score += value;
		}
	}
	score += captures * 1000;
	return score;
}

int Evaluator::_emptyPattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE)
{
	return 0;
}

int Evaluator::__o_Pattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	return 0;
	unsigned int lx = GobanIterator::direction[dir][0] * 2 + x;
	unsigned int ly = GobanIterator::direction[dir][1] * 2 + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return 20;
	return 0;
}
int Evaluator::__ooPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	return 0;
	unsigned int lx = GobanIterator::direction[dir][0] * 2 + x;
	unsigned int ly = GobanIterator::direction[dir][1] * 2 + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return 45;
	return -25;
}
int Evaluator::__oo_Pattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	return 0;
	unsigned int lx = GobanIterator::direction[dir][0] * 2 + x;
	unsigned int ly = GobanIterator::direction[dir][1] * 2 + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return 30;
	return -10;
}
int Evaluator::__ooxPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	return 0;
	unsigned int lx = GobanIterator::direction[dir][0] * 2 + x;
	unsigned int ly = GobanIterator::direction[dir][1] * 2 + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return -25;
	return 45;
}
int Evaluator::__oooPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	return 0;
	unsigned int lx = GobanIterator::direction[dir][0] * 2 + x;
	unsigned int ly = GobanIterator::direction[dir][1] * 2 + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return 35;
	return -15;
}
int Evaluator::_o_Pattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	unsigned int lx = GobanIterator::direction[dir][0] * 2 + x;
	unsigned int ly = GobanIterator::direction[dir][1] * 2 + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return 20;
	return 0;
}
int Evaluator::_o_o_Pattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	return 0;
	unsigned int lx = GobanIterator::direction[dir][0] * 2 + x;
	unsigned int ly = GobanIterator::direction[dir][1] * 2 + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return 30;
	return -10;
}
int Evaluator::_oxPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	unsigned int lx = GobanIterator::direction[dir][0] + x;
	unsigned int ly = GobanIterator::direction[dir][1] + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return -10;
	return 30;
}
int Evaluator::_ooPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	unsigned int lx = GobanIterator::direction[dir][0] + x;
	unsigned int ly = GobanIterator::direction[dir][1] + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return 25;
	return 5;
}
 int Evaluator::_oo_Pattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	unsigned int lx = GobanIterator::direction[dir][0] + x;
	unsigned int ly = GobanIterator::direction[dir][1] + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return 30;
	return -10;
}
int Evaluator::_ooxPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	return 0;
	unsigned int lx = GobanIterator::direction[dir][0] + x;
	unsigned int ly = GobanIterator::direction[dir][1] + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return -15;
	return 35;
}
int Evaluator::_oooPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	unsigned int lx = GobanIterator::direction[dir][0] + x;
	unsigned int ly = GobanIterator::direction[dir][1] + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return 40;
	return -20;
}
int Evaluator::_ooo_Pattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	unsigned int lx = GobanIterator::direction[dir][0] + x;
	unsigned int ly = GobanIterator::direction[dir][1] + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return 50;
	return -30;
}
int Evaluator::_oooxPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	unsigned int lx = GobanIterator::direction[dir][0] + x;
	unsigned int ly = GobanIterator::direction[dir][1] + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return 45;
	return -25;
}
int Evaluator::_ooooPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	
	unsigned int lx = GobanIterator::direction[dir][0] + x;
	unsigned int ly = GobanIterator::direction[dir][1] + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return 60;
	return -35;
}

