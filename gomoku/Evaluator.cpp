#include "Evaluator.h"
#include "GobanIterator.h"
#include "PatternIdentifier.h"

struct ThreatSearch
{
	int nextEval;
	int score;
	int (*evaluator)(Goban::PION_TYPE p,Goban::Case,Goban::Case,Goban&,unsigned int,unsigned int,int);
};


Evaluator::Evaluator(Goban const * g) : _base(g)
{
}


Evaluator::~Evaluator(void)
{
}

inline int fiveInRow(Goban::PION_TYPE p, Goban::Case pattern1, Goban::Case pattern2, Goban & g, unsigned int x, unsigned int y, int d)
{
	unsigned int lx = x + GobanIterator::direction[d][0];
	unsigned int ly = y + GobanIterator::direction[d][1];
	if (pattern1 == Patterns::oooo && (g[ly][lx] & Goban::PIONMASK) == p)
		return 1;
	return 0;
}

inline int straightFour(Goban::PION_TYPE p, Goban::Case pattern1, Goban::Case pattern2, Goban & g, unsigned int x, unsigned int y, int d)
{
	unsigned int lx = x + GobanIterator::direction[d][0];
	unsigned int ly = y + GobanIterator::direction[d][1];
	if (pattern1 == Patterns::ooox && (g[ly][lx] & Goban::PIONMASK) == p)
		return 1;
	return 0;
}

inline int fourInRow(Goban::PION_TYPE p, Goban::Case pattern1, Goban::Case pattern2, Goban & g, unsigned int x, unsigned int y, int d)
{
	unsigned int lx = x + GobanIterator::direction[d][0];
	unsigned int ly = y + GobanIterator::direction[d][1];
	if (pattern1 == Patterns::ooo_ && (g[ly][lx] & Goban::PIONMASK) == p)
		return 1;
	return 0;
}

inline int threeInRow(Goban::PION_TYPE p, Goban::Case pattern1, Goban::Case pattern2, Goban & g, unsigned int x, unsigned int y, int d)
{
	unsigned int lx = x + GobanIterator::direction[d][0];
	unsigned int ly = y + GobanIterator::direction[d][1];
	if (pattern1 == Patterns::oo_ && (g[ly][lx] & Goban::PIONMASK) == p)
		return 1;
	return 0;
}

inline int brokenThree(Goban::PION_TYPE p, Goban::Case pattern1, Goban::Case pattern2, Goban & g, unsigned int x, unsigned int y, int d)
{
	unsigned int lx = x + GobanIterator::direction[d][0] * 3;
	unsigned int ly = y + GobanIterator::direction[d][1] * 3;
	if (!g.InBound(lx, ly)) return 0;
	if ((pattern1 == Patterns::o_o_ || pattern1 == Patterns::_oo_) && (g[ly][lx] & Goban::PIONMASK) == p)
		return 1;
	return 0;
}

inline int twoInRow(Goban::PION_TYPE p, Goban::Case pattern1, Goban::Case pattern2, Goban & g, unsigned int x, unsigned int y, int d)
{
	unsigned int lx = x + GobanIterator::direction[d][0];
	unsigned int ly = y + GobanIterator::direction[d][1];
	if (pattern1 == Patterns::o_ && (g[ly][lx] & Goban::PIONMASK) == p)
		return 1;
	return 0;
}

inline int singleMark(Goban::PION_TYPE, Goban::Case, Goban::Case, Goban &, unsigned int, unsigned int, int)
{
	return 1;
}

const ThreatSearch threatSearchs[7] = {
	{0, 1000, fiveInRow},
	{0, 400, straightFour},
	{0, 250, fourInRow},
	{0, 200, threeInRow},
	{5, 150, brokenThree},
	{3, 20, twoInRow},
	{2, 0, singleMark},
};

int Evaluator::operator()(Goban & g, Goban::PION_TYPE p)
{   
	Goban::PION_TYPE currentPion;
	Goban::PION_TYPE other = Goban::Other(p);
	int score = 0;
	int captures = g.deletedStone(other) - this->_base->deletedStone(p);
	int threat[7][2] = {0};
	Goban::Case * current = g[0];
	Goban::Case toEval;

	for (unsigned int x = 0, y = 0; y < 19; ++current)
	{
		toEval = *current;
		currentPion = (Goban::PION_TYPE)(toEval & Goban::PIONMASK);
		toEval >>= Goban::HEADERSIZE;
		if (currentPion)
		{
			for (int d = 0, i = 0; d < 8;)
			{
				if (threatSearchs[i].evaluator(currentPion, toEval & Goban::PATTERNMASK, 0, g, x, y, d))
				{
					if (currentPion == p) score += threatSearchs[i].score;
					else score -= threatSearchs[i].score;
					i = 0;
					++d;
					toEval >>= Goban::PATTERNSIZE;
				}
				else ++i;
			}
		}
		if (++x >= 19)
		{
			x = 0;
			++y;
		}
	}
	//score += captures * 10;
	return score;
}

inline int Evaluator::influence(Goban::Case c, Goban &, unsigned int, unsigned int, Goban::PION_TYPE p)
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

inline int Evaluator::alignments(Goban::Case c, Goban & g, unsigned int x, unsigned int y, Goban::PION_TYPE p)
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
		{ Patterns::ooox, 3, 1, true, 150},
		{ Patterns::oooo, 4, 1, true, 200}
	};

	int score = 0;
	unsigned int lx, ly;
	Goban::Case p1, p2;
	Goban::PION_TYPE pion = static_cast<Goban::PION_TYPE>(c & Goban::PIONMASK);
	c >>= Goban::HEADERSIZE;
	for (int d = 0; d < 4; ++d)
	{
		p1 = c & Goban::PATTERNMASK;
		p2 = (c >> (Goban::PATTERNSIZE * 4)) & Goban::PATTERNMASK;
		const PatternInfos * __restrict pi1 = (p1 >= Patterns::_o_ && p1 <= Patterns::oooo) ? patternInfos + p1 - 1 : 0; 
		const PatternInfos * __restrict pi2 = (p2 >= Patterns::_o_ && p2 <= Patterns::oooo) ? patternInfos + p2 - 1 : 0; 
		int align = 0;
		if (pi1)
		{
			lx = x + GobanIterator::direction[d][0] * pi1->firstCaseIndex;
			ly = y + GobanIterator::direction[d][1] * pi1->firstCaseIndex;
			if ((g[ly][lx] & Goban::PIONMASK) == pion) align += pi1->align;
		}
		if (pi2)
		{
			lx = x + GobanIterator::direction[d][0] * pi2->firstCaseIndex;
			ly = y + GobanIterator::direction[d][1] * pi2->firstCaseIndex;
			if ((g[ly][lx] & Goban::PIONMASK) == pion) align += pi2->align;
		}
		switch (align)
		{
		case 0: break;
		case 1: 
			score += (pion==p) ? align : -align;
			break;
		case 2: 
			score += (pion==p) ? align : -align;
			break;
		case 3:
			score += (pion==p) ? align : -500;
			break;
		case 4:
			score += (pion==p) ? 400: -1000;
			break;
		default:
			score += (pion==p) ? 1000 : -2000;
			break;
		}
		c >>= Goban::PATTERNSIZE;
	}
	return score;
}