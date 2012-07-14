#include "Evaluator.h"
#include "GobanIterator.h"
#include "PatternIdentifier.h"
#include "XYManager.h"

Evaluator::Evaluator(Goban const * g) : _base(g)
{
}


Evaluator::~Evaluator(void)
{
}

int ThreatAlign[5][2][2] = {
	{{0,3}, {3,5}},
	{{0,65}, {65,75}},
	{{0,200}, {200,250}},
	{{0,900}, {900,1000}},
	{{0,2000}, {2000,2500}},
};

inline int GetThreatScore(Goban::PION_TYPE p, Goban::Case c, Goban & g, unsigned int x, unsigned int y, int d, int & nextEval)
{
	const Patterns::PatternInfos * pInfos1 = Patterns::patterns + ((c >> Goban::HEADERSIZE >> Goban::PATTERNSIZE * d) & Goban::PATTERNMASK);
	const Patterns::PatternInfos * pInfos2 = Patterns::patterns + ((c >> Goban::HEADERSIZE >> Goban::PATTERNSIZE * (d+4)) & Goban::PATTERNMASK);
	nextEval = pInfos1->size;
	if (!p)
	{
		nextEval = (pInfos1->pattern) ? pInfos1->caseIndex - 1 : 4;
		return 0;
	}
	unsigned int lx = x + pInfos1->caseIndex * GobanIterator::direction[d][0];
	unsigned int ly = y + pInfos1->caseIndex * GobanIterator::direction[d][1];
	int align = 0;
	int expand = 0;
	if ((g[ly][lx] & Goban::PIONMASK) != p)
		nextEval = pInfos1->caseIndex - 1;
	if (g.InBound(lx, ly) && (g[ly][lx] & Goban::PIONMASK) == p)
	{
		align += pInfos1->size - pInfos1->expand;
		expand += pInfos1->expand;
	}
	lx = x - pInfos2->caseIndex * GobanIterator::direction[d][0];
	ly = y - pInfos2->caseIndex * GobanIterator::direction[d][1];
	if (g.InBound(lx, ly) && (g[ly][lx] & Goban::PIONMASK) == p)
	{
		align += pInfos2->size - pInfos2->expand;
		expand += pInfos2->expand;
	}

	if (align + expand >= 4)
	{
		int maxalign = (align <= 4) ? align : 4;
		//qDebug() << maxalign  << x << y;
		return ThreatAlign[maxalign][pInfos1->free][pInfos2->free];
	}
	return 0;
}

inline int canCreateCapture(Goban::PION_TYPE p, Goban::Case c, Goban & g, unsigned int x, unsigned int y, int d)
{
	const Patterns::PatternInfos * pInfos1 = Patterns::patterns + ((c >> Goban::HEADERSIZE >> Goban::PATTERNSIZE * d) & Goban::PATTERNMASK);
	const Patterns::PatternInfos * pInfos2 = Patterns::patterns + ((c >> Goban::HEADERSIZE >> Goban::PATTERNSIZE * (d+4)) & Goban::PATTERNMASK);
	int capture = 0;
	unsigned int lx = x + pInfos1->caseIndex * GobanIterator::direction[d][0];
	unsigned int ly = y + pInfos1->caseIndex * GobanIterator::direction[d][1];
	if ((g[ly][lx] & Goban::PIONMASK) != p && pInfos1->align == 2 && pInfos1->free)
		++capture;
	lx = x + pInfos2->caseIndex * GobanIterator::direction[d][0];
	ly = y + pInfos2->caseIndex * GobanIterator::direction[d][1];
	if ((g[ly][lx] & Goban::PIONMASK) != p && pInfos2->align == 2 && pInfos2->free)
		++capture;
	return capture;
}

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
		if (currentPion || 1)
		{
			score += (currentPion == p) ? GetThreatScore(currentPion, toEval, g, x, y, dir, nextEval) :
						-GetThreatScore(currentPion, toEval, g, x, y, dir, nextEval) * 1.25;
			if (currentPion)
				score += ((currentPion == p) ? 100: -100) * canCreateCapture(currentPion, toEval, g, x, y, dir);
		}
		else
		{
			//score += pow(5.0, Goban::GetInfluence(toEval, p));
			//score -= pow(5.0, Goban::GetInfluence(toEval, Goban::Other(p)));
			nextEval = 4;
		}
		manager.incremente(x, y, ++nextEval);
	}
}

int Evaluator::operator()(Goban & g, Goban::PION_TYPE p)
{   
	Goban::PION_TYPE currentPion;
	Goban::PION_TYPE other = Goban::Other(p);
	int score = 0;
	Goban::Case toEval;
	eval_case<0>(score, g, toEval, currentPion, p);
	eval_case<1>(score, g, toEval, currentPion, p);
	eval_case<2>(score, g, toEval, currentPion, p);
	eval_case<3>(score, g, toEval, currentPion, p);
	//score += (g.deletedStone(p) - this->_base->deletedStone(p)) * 1000;
	//score -= (g.deletedStone(Goban::Other(p)) - this->_base->deletedStone(Goban::Other(p))) * 1000;
	return score;
}

inline int Evaluator::influence(Goban::Case c, Goban &, unsigned int, unsigned int, Goban::PION_TYPE p)
{
	return Goban::GetInfluence(c, p) * Goban::GetInfluence(c, p) * Goban::GetInfluence(c, p);
}
