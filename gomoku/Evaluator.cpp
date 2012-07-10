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

/*
const ThreatSearch threatSearchs[8] = {
	{0, 5000, fiveInRow},
	{0, 2000, straightFour},
	{0, 1500, fourInRow},
	{0, -1000, canBeCaptured},
	{0, 750, threeInRow},
	{5, 500, brokenThree},
	{3, 50, twoInRow},
	{2, 1, singleMark},
};*/

int GetThreatScore(Goban::PION_TYPE p, Goban::Case c, Goban & g, unsigned int x, unsigned int y, int d, int & nextEval)
{
	Patterns::PatternInfos pInfos1 = Patterns::patterns[(c >> Goban::HEADERSIZE >> Goban::PATTERNSIZE * d) & Goban::PATTERNMASK];
	Patterns::PatternInfos pInfos2 = Patterns::patterns[(c >> Goban::HEADERSIZE >> Goban::PATTERNSIZE * (d+4)) & Goban::PATTERNMASK];
	nextEval = pInfos1.size;
	unsigned int lx = x + pInfos1.caseIndex * GobanIterator::direction[d][0];
	unsigned int ly = y + pInfos1.caseIndex * GobanIterator::direction[d][1];
	int align = 0;
	int expand = 0;
	if (g.InBound(lx, ly) && (g[ly][lx] & Goban::PIONMASK) == p)
	{
		align += pInfos1.align;
		expand += pInfos1.expand;
	}
	lx = x - pInfos2.caseIndex * GobanIterator::direction[d][0];
	ly = y - pInfos2.caseIndex * GobanIterator::direction[d][1];
	if (g.InBound(lx, ly) && (g[ly][lx] & Goban::PIONMASK) == p)
	{
		align += pInfos2.align;
		expand += pInfos2.expand;
	}
	if (align + expand >= 4)
	{
		switch (align)
		{
		case 0: return 5;
		case 1: return 50;
		case 2: return 500;
		case 3: return 5000;
		case 4: return 50000;
		}
	}
	return 0;
}

template<int dir> struct XYManager { };
template<> struct XYManager<0>
{ 
	inline void init(int &x, int &y)
	{
		x = 0;
		y = 18;
	}

	inline bool ended(int &x, int &)
	{
		return x >= 19;
	}

	inline void incremente(int &x, int &y, int & add)
	{
		y += add;
		if (y >= 19)
		{
			x++;
			y = 0;
		}
	}
};

template<> struct XYManager<1>
{ 
	inline void init(int &x, int &y)
	{
		x = 0;
		y = 0;
		this->_line = 0;
	}

	inline bool ended(int &x, int &y)
	{
		return this->_line >= 40;
	}

	inline void incremente(int &x, int &y, int & add)
	{
		bool overflow = false;
		y -= add;
		x += add;
		if (x >= 19 || y < 0)
			overflow = true;
		if (overflow)
		{
			y = ++this->_line;
			if (y >= 19)
			{
				x = y % 18;
				y = 18;
			}
		}
	}

private:
	int _line;
};

template<> struct XYManager<2>
{ 
	static inline void init(int &x, int &y)
	{
		x = 0;
		y = 0;
	}

	static inline bool ended(int &, int &y)
	{
		return y >= 19;
	}

	static inline void incremente(int &x, int &y, int & add)
	{
		x += add;
		if (x >= 19)
		{
			y++;
			x = 0;
		}
	}
};

template<> struct XYManager<3>
{ 
	inline void init(int &x, int &y)
	{
		x = 0;
		y = 18;
		this->_line = 0;
	}

	inline bool ended(int &x, int &y)
	{
		return this->_line >= 40;
	}

	inline void incremente(int &x, int &y, int & add)
	{
		bool overflow = false;
		y += add;
		x += add;
		if (x >= 19 || y >= 19)
			overflow = true;
		if (overflow)
		{
			y = 18 - ++this->_line;
			if (y < 0)
			{
				x = - y;
				y = 0;
			}
		}
	}

private:
	int _line;
};

template<int dir>
void eval_case(int &score, Goban &g, Goban::Case &toEval, Goban::PION_TYPE & currentPion, Goban::PION_TYPE &p)
{
	int x = 0, y = 0;
	XYManager<dir> manager;
	manager.init(x, y);
	int nextEval = 0;

	while (!manager.ended(x, y))
	{
		toEval = g[y][x];
		currentPion = (Goban::PION_TYPE)(toEval & Goban::PIONMASK);
		if (currentPion)
			score += (currentPion == p) ? GetThreatScore(currentPion, toEval, g, x, y, dir, nextEval) :
						-GetThreatScore(currentPion, toEval, g, x, y, dir, nextEval);
		else nextEval = 0;
		manager.incremente(x, y, ++nextEval);
	}
}

int Evaluator::operator()(Goban & g, Goban::PION_TYPE p)
{   
	Goban::PION_TYPE currentPion;
	Goban::PION_TYPE other = Goban::Other(p);
	int score = 0;
	int captures = g.deletedStone(other) - this->_base->deletedStone(p);
	Goban::Case toEval;

	eval_case<0>(score, g, toEval, currentPion, p);
	eval_case<1>(score, g, toEval, currentPion, p);
	eval_case<2>(score, g, toEval, currentPion, p);
	eval_case<3>(score, g, toEval, currentPion, p);

	score += captures * 1000;
	return score;
}

inline int Evaluator::influence(Goban::Case c, Goban &, unsigned int, unsigned int, Goban::PION_TYPE p)
{
	return Goban::GetInfluence(c, p) * Goban::GetInfluence(c, p) * Goban::GetInfluence(c, p);
}
