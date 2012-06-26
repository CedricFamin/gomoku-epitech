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
	for (unsigned int y = 0; y < 19; ++y)
	{
		for (unsigned int x = 0; x < 19; ++x)
		{
			current = g[y][x];
			currentPion = (Goban::PION_TYPE)(current & Goban::PIONMASK);
			if (currentPion == 0)
			{
				score += influence(current, g, x, y, p);
				//score -= influence(g, x, y, Goban::Other(p));
				
			}
			else
				score += alignments(current, g, x, y, p);
		}
	}
	score += captures * 10;
	return score;
}

inline int Evaluator::influence(Goban::Case c, Goban & g, unsigned int x, unsigned int y, Goban::PION_TYPE p)
{
	return Goban::GetInfluence(c, p) * Goban::GetInfluence(c, p) * Goban::GetInfluence(c, p);
}

struct PatternInfos
{
	Patterns::Patterns pattern;
	unsigned int align;
	int firstCaseIndex;
	bool blocked;
	int score;
};

inline const PatternInfos * GetPatternInfos(Goban::Case pattern)
{
	static const PatternInfos patternInfos[15] = {
		{ Patterns::_o_, 1, 2, false, 0},
		{ Patterns::_oo, 2, 2, true, 50},
		{ Patterns::_oo_, 2, 2, false, 60},
		{ Patterns::_oox, 2, 2, true, -100},
		{ Patterns::_ooo, 3, 2, true, 80},
		{ Patterns::o_, 1, 1, false, 0},
		{ Patterns::o_o_, 2, 1, false, 40},
		{ Patterns::ox, 1, 1, true, -20},
		{ Patterns::oo, 1, 1, true, 40},
		{ Patterns::oo_, 2, 1, false, 50},
		{ Patterns::oox, 2, 1, true, -100},
		{ Patterns::ooo, 3, 1, true, 75},
		{ Patterns::ooo_, 3, 1, false, 80},
		{ Patterns::ooox, 3, 1, true, 70},
		{ Patterns::oooo, 4, 1, true, 200}
	};
	if (pattern == 0) return 0;
	if (pattern >= Patterns::_o_ && pattern <= Patterns::oooo)
		return patternInfos + pattern - 1;
	return 0;
}

inline int Evaluator::alignments(Goban::Case c, Goban & g, unsigned int x, unsigned int y, Goban::PION_TYPE p)
{
	int score = 0;
	unsigned int lx, ly;
	const PatternInfos *pi;
	c >>= Goban::HEADERSIZE;
	for (int d = 0; d < 8; ++d)
	{
		pi = GetPatternInfos(c & Goban::PATTERNMASK);
		if (pi)
		{
			lx = x + GobanIterator::direction[d][0] * pi->firstCaseIndex;
			ly = y + GobanIterator::direction[d][1] * pi->firstCaseIndex;
			if (p == (g[ly][lx] & Goban::PIONMASK))
				score += pi->score;
			else score -= pi->score;
		}
		c >>= Goban::PATTERNSIZE;
	}
	return score;
}