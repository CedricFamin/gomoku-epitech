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
	{{0,15}, {15,25}},
	{{0,100}, {100,125}},
	{{0,500}, {500,625}},
	{{0,2500}, {2500,3125}},
};

template<int d>
int AddAlignExpand(int &align, int &expand, bool & expandable, Goban & g, unsigned int x, unsigned int y, int const p, Patterns::PatternInfos const * pInfos)
{
	ChangeCase<d>(x, y, pInfos->caseIndex);
	if (g.InBound(x, y) && (g[y][x] & Goban::PIONMASK) == p)
	{
		align += pInfos->size - pInfos->expand;
		expand += pInfos->expand;
		expandable = pInfos->expand;
		return pInfos->size + 1;
	}
	expand += pInfos->caseIndex - 1;
	expandable = pInfos->caseIndex - 1;
	return pInfos->caseIndex;
}

template<int d>
inline int GetThreatScore(Goban::PION_TYPE p, Goban::Case c, Goban & g, unsigned int x, unsigned int y, int & nextEval)
{
	const Patterns::PatternInfos * pInfos1 = Patterns::patterns + ((c >> Goban::HEADERSIZE >> Goban::PATTERNSIZE * d) & Goban::PATTERNMASK);
	const Patterns::PatternInfos * pInfos2 = Patterns::patterns + ((c >> Goban::HEADERSIZE >> Goban::PATTERNSIZE * (d+4)) & Goban::PATTERNMASK);
	bool e1, e2;
	if (!p)
	{
		nextEval = (pInfos1->pattern) ? pInfos1->caseIndex : 5;
		return 0;
	}
	int align = 0;
	int expand = 0;
	nextEval = AddAlignExpand<d>(align, expand, e1, g, x, y, p, pInfos1);
	AddAlignExpand<d+4>(align, expand, e2, g, x, y, p, pInfos2);
	if (align + expand >= 4)
	{
		int maxalign = (align <= 4) ? align : 4;
		return ThreatAlign[maxalign][e1][e2];
	}
	return 0;
}

template<int d>
inline int canCreateCapture(Goban::PION_TYPE p, Goban::Case c, Goban & g, unsigned int x, unsigned int y)
{
	const Patterns::PatternInfos * pInfo = Patterns::patterns + ((c >> Goban::HEADERSIZE >> Goban::PATTERNSIZE * d) & Goban::PATTERNMASK);
	unsigned int lx = x, ly = y;
	ChangeCase<d>(lx, ly, pInfo->caseIndex);
	if ((g[ly][lx] & Goban::PIONMASK) != p && pInfo->align == 2 && pInfo->expand)
		return 1;
	return 0;
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
		score += (currentPion == p) ? GetThreatScore<dir>(currentPion, toEval, g, x, y, nextEval) :
					-GetThreatScore<dir>(currentPion, toEval, g, x, y, nextEval);
		if (currentPion)
		{
			score += ((currentPion == p) ? 250: -250) * canCreateCapture<dir>(currentPion, toEval, g, x, y);
			score += ((currentPion == p) ? 250: -250) * canCreateCapture<dir+4>(currentPion, toEval, g, x, y);
		}
		manager.incremente(x, y, nextEval);
	}
}

int Evaluator::operator()(Goban & g, Goban::PION_TYPE p)
{   
	Goban::PION_TYPE currentPion;
	int score = 0;
	Goban::Case toEval;
	eval_case<0>(score, g, toEval, currentPion, p);
	eval_case<1>(score, g, toEval, currentPion, p);
	eval_case<2>(score, g, toEval, currentPion, p);
	eval_case<3>(score, g, toEval, currentPion, p);
	score -= g.deletedStone(p) * 500;
	score += g.deletedStone(Goban::Other(p)) * 500;
	return score;
}

inline int Evaluator::influence(Goban::Case c, Goban &, unsigned int, unsigned int, Goban::PION_TYPE p)
{
	return Goban::GetInfluence(c, p) * Goban::GetInfluence(c, p) * Goban::GetInfluence(c, p);
}
