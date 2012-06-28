#include "Evaluator.h"
#include "GobanIterator.h"
#include "PatternIdentifier.h"

struct ThreatSearch
{
	int nextEval;
	int score;
	bool (*evaluator)(Goban::PION_TYPE p,Goban::Case,Goban&,unsigned int,unsigned int,int);
};


Evaluator::Evaluator(Goban const * g) : _base(g)
{
}


Evaluator::~Evaluator(void)
{
}

bool deadLine(Goban::PION_TYPE p, Goban &g, unsigned int x, unsigned int y, int d, int needed)
{
	return true;
	int free = 1;
	unsigned int lx = x + GobanIterator::direction[d][0];
	unsigned int ly = y + GobanIterator::direction[d][1];
		
	while (g.InBound(lx, ly))
	{
		if ((g[ly][lx] & Goban::PIONMASK) == p
			|| (g[ly][lx] & Goban::PIONMASK) == 0)
			free++;
		else break;
		if (free >= 5) return true;
		lx = x + GobanIterator::direction[d][0];
		ly = y + GobanIterator::direction[d][1];
	}
	lx = x - GobanIterator::direction[d][0];
	ly = y - GobanIterator::direction[d][1];
	while (g.InBound(lx, ly))
	{
		if ((g[ly][lx] & Goban::PIONMASK) == p
			|| (g[ly][lx] & Goban::PIONMASK) == 0)
			free++;
		else break;
		if (free >= 5) return true;
		lx = x - GobanIterator::direction[d][0];
		ly = y - GobanIterator::direction[d][1];
	}
	return free >= 5;

}

inline bool fiveInRow(Goban::PION_TYPE p, Goban::Case c, Goban & g, unsigned int x, unsigned int y, int d)
{
	Goban::Case pattern = (c >> Goban::HEADERSIZE >> (Goban::PATTERNSIZE * d)) & Goban::PATTERNMASK;
	unsigned int lx = x + GobanIterator::direction[d][0];
	unsigned int ly = y + GobanIterator::direction[d][1];
	if (pattern == Patterns::oooo && (g[ly][lx] & Goban::PIONMASK) == p)
		return true;
	return false;
}

inline bool straightFour(Goban::PION_TYPE p, Goban::Case c, Goban & g, unsigned int x, unsigned int y, int d)
{
	Goban::Case pattern = (c >> Goban::HEADERSIZE >> (Goban::PATTERNSIZE * d)) & Goban::PATTERNMASK;
	unsigned int lx = x + GobanIterator::direction[d][0];
	unsigned int ly = y + GobanIterator::direction[d][1];
	if (pattern == Patterns::ooox && (g[ly][lx] & Goban::PIONMASK) == p)
		return deadLine(p, g, x, y, d, 5);
	return false;
}

inline bool fourInRow(Goban::PION_TYPE p, Goban::Case c, Goban & g, unsigned int x, unsigned int y, int d)
{
	Goban::Case pattern = (c >> Goban::HEADERSIZE >> (Goban::PATTERNSIZE * d)) & Goban::PATTERNMASK;
	unsigned int lx = x + GobanIterator::direction[d][0];
	unsigned int ly = y + GobanIterator::direction[d][1];
	if (pattern == Patterns::ooo_ && (g[ly][lx] & Goban::PIONMASK) == p)
		return deadLine(p, g, x, y, d, 5);
	return false;
}

inline bool threeInRow(Goban::PION_TYPE p, Goban::Case c, Goban & g, unsigned int x, unsigned int y, int d)
{
	Goban::Case pattern = (c >> Goban::HEADERSIZE >> (Goban::PATTERNSIZE * d)) & Goban::PATTERNMASK;
	unsigned int lx = x + GobanIterator::direction[d][0];
	unsigned int ly = y + GobanIterator::direction[d][1];
	if (pattern == Patterns::oo_ && (g[ly][lx] & Goban::PIONMASK) == p)
		return deadLine(p, g, x, y, d, 5);
	return false;
}

inline bool brokenThree(Goban::PION_TYPE p, Goban::Case c, Goban & g, unsigned int x, unsigned int y, int d)
{
	Goban::Case pattern = (c >> Goban::HEADERSIZE >> (Goban::PATTERNSIZE * d)) & Goban::PATTERNMASK;
	unsigned int lx = x + GobanIterator::direction[d][0] * 3;
	unsigned int ly = y + GobanIterator::direction[d][1] * 3;
	if (!g.InBound(lx, ly)) return 0;
	if ((pattern == Patterns::o_o_ || pattern == Patterns::_oo_) && (g[ly][lx] & Goban::PIONMASK) == p)
		return deadLine(p, g, x, y, d, 5);
	return false;
}

inline bool twoInRow(Goban::PION_TYPE p, Goban::Case c, Goban & g, unsigned int x, unsigned int y, int d)
{
	Goban::Case pattern = (c >> Goban::HEADERSIZE >> (Goban::PATTERNSIZE * d)) & Goban::PATTERNMASK;
	unsigned int lx = x + GobanIterator::direction[d][0];
	unsigned int ly = y + GobanIterator::direction[d][1];
	if (pattern == Patterns::o_ && (g[ly][lx] & Goban::PIONMASK) == p)
	{
		return deadLine(p, g, x, y, d, 5);
	}
	return false;
}

inline bool singleMark(Goban::PION_TYPE p, Goban::Case, Goban & g, unsigned int x, unsigned int y, int d)
{
	return deadLine(p, g, x, y, d, 5);
}

inline bool canBeCaptured(Goban::PION_TYPE p, Goban::Case c, Goban & g, unsigned int x, unsigned int y, int d)
{
	Goban::Case pattern = (c >> Goban::HEADERSIZE >> (Goban::PATTERNSIZE * d)) & Goban::PATTERNMASK;
	unsigned int lx = x + GobanIterator::direction[d][0];
	unsigned int ly = y + GobanIterator::direction[d][1];
	if (pattern == Patterns::ox && (g[ly][lx] & Goban::PIONMASK) == p)
	{
		lx = x - GobanIterator::direction[d][0];
		ly = y - GobanIterator::direction[d][1];
		if (g.InBound(lx, ly) && (g[ly][lx] & Goban::PIONMASK) == 0)
			return deadLine(p, g, x, y, d, 5);
	}
	d+=4;
	pattern = (c >> Goban::HEADERSIZE >> (Goban::PATTERNSIZE * d)) & Goban::PATTERNMASK;
	lx = x + GobanIterator::direction[d][0];
	ly = y + GobanIterator::direction[d][1];
	if (pattern == Patterns::ox && (g[ly][lx] & Goban::PIONMASK) == p)
	{
		lx = x - GobanIterator::direction[d][0];
		ly = y - GobanIterator::direction[d][1];
		if (g.InBound(lx, ly) && (g[ly][lx] & Goban::PIONMASK) == 0)
			return deadLine(p, g, x, y, d, 5);
	}
	return false;
}

const ThreatSearch threatSearchs[8] = {
	{0, 5000, fiveInRow},
	{0, 2000, straightFour},
	{0, 1500, fourInRow},
	{0, -1000, canBeCaptured},
	{0, 750, threeInRow},
	{5, 500, brokenThree},
	{3, 50, twoInRow},
	{2, 1, singleMark},
};

int Evaluator::operator()(Goban & g, Goban::PION_TYPE p)
{   
	Goban::PION_TYPE currentPion;
	Goban::PION_TYPE other = Goban::Other(p);
	int score = 0;
	int captures = g.deletedStone(other) - this->_base->deletedStone(p);
    //int threat[7][2] = {0, 0};
	Goban::Case * current = g[0];
	Goban::Case toEval;

	for (unsigned int x = 0, y = 0; y < 19; ++current)
	{
		toEval = *current;
		currentPion = (Goban::PION_TYPE)(toEval & Goban::PIONMASK);
		toEval >>= Goban::HEADERSIZE;
		if (currentPion)
		{
			for (unsigned int d = 0, i = 0; d < 4;)
			{
				if (threatSearchs[i].evaluator(currentPion, *current, g, x, y, d))
				{
					if (currentPion == p) score += threatSearchs[i].score;
					else score -= threatSearchs[i].score;
					i = 0;
					++d;
					toEval >>= Goban::PATTERNSIZE;
				}
				else ++i;
				if (i >= (sizeof(threatSearchs) / sizeof(*threatSearchs))) i = 0;
			}
		}
		if (++x >= 19)
		{
			x = 0;
			++y;
		}
	}
	score += captures * 750;
	return score;
}

inline int Evaluator::influence(Goban::Case c, Goban &, unsigned int, unsigned int, Goban::PION_TYPE p)
{
	return Goban::GetInfluence(c, p) * Goban::GetInfluence(c, p) * Goban::GetInfluence(c, p);
}
