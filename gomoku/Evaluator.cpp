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

 int GetThreatScore(Goban::PION_TYPE p, Goban::Case c, Goban & g, unsigned int x, unsigned int y, int d)//, int & nextEval)
{
	Patterns::PatternInfos pInfos1 = Patterns::patterns[(c >> Goban::HEADERSIZE >> Goban::PATTERNSIZE * d) & Goban::PIONMASK];
	Patterns::PatternInfos pInfos2 = Patterns::patterns[(c >> Goban::HEADERSIZE >> Goban::PATTERNSIZE * (d+4)) & Goban::PIONMASK];
	unsigned int lx = x + pInfos1.caseIndex * GobanIterator::direction[d][0];
	unsigned int ly = y + pInfos1.caseIndex * GobanIterator::direction[d][0];
	int align = 0;
	align += (g.InBound(lx, ly) && (g[ly][lx] & Goban::PIONMASK) == p) ? pInfos1.align : 0;
	lx = x - pInfos2.caseIndex * GobanIterator::direction[d][0];
	ly = y - pInfos2.caseIndex * GobanIterator::direction[d][0];
	align += (g.InBound(lx, ly) && (g[ly][lx] & Goban::PIONMASK) == p) ? pInfos2.align : 0;
	switch (align)
	{
	case 0: return 1;
	case 1: return 50;
	case 2: return 750;
	case 3: return 2000;
	case 4: return 5000;
	default: return 6000;
	}
	return 0;
}

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
		if (currentPion)
		{
			for (unsigned int d = 0, i = 0; d < 4; ++d)
			{
				score += (currentPion == p) ? GetThreatScore(currentPion, toEval, g, x, y, d) :
					-GetThreatScore(currentPion, toEval, g, x, y, d);
			}
		}
		if (++x >= 19)
		{
			x = 0;
			++y;
		}
	}
	//score += captures * 1000;
	return score;
}

inline int Evaluator::influence(Goban::Case c, Goban &, unsigned int, unsigned int, Goban::PION_TYPE p)
{
	return Goban::GetInfluence(c, p) * Goban::GetInfluence(c, p) * Goban::GetInfluence(c, p);
}
