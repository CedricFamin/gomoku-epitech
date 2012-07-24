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

template<int a, int b>
struct Min
{
	static const int v = (a > b) ? b : a;
};

template<int nb, int p>
struct Pow
{
	static const int v = nb * Pow<nb, p - 1>::v;
};

template<int nb>
struct Pow<nb, 0>
{
	static const int v = 1;
};

template<bool minus, int a, int ex, bool de>
struct MinusScoring
{
	static const int content = Pow<5, Min<5, a + ex>::v>::v;
	static const int align = Pow<5, a>::v;
};

template<int a, int ex, bool de>
struct MinusScoring<false, a, ex, de>
{
	static const int content = Pow<5, Min<5, a + ex>::v - !de>::v;
	static const int align = Pow<5, a - !de>::v;
};

template<int a, int ex, bool de>
struct Scoring
{
	static const int content = MinusScoring<a>=4, a, ex, de>::content;
	static const int align = MinusScoring<a>=4, a, ex, de>::align;
	static const int score = content + align;
};

int Scores[5][5][2] = {
	{
		{Scoring<1,0,0>::score, Scoring<1,0,1>::score},
		{Scoring<1,1,0>::score, Scoring<1,1,1>::score},
		{Scoring<1,2,0>::score, Scoring<1,2,1>::score},
		{Scoring<1,3,0>::score, Scoring<1,3,1>::score},
		{Scoring<1,4,0>::score, Scoring<1,4,1>::score},
	}, {
		{Scoring<2,0,0>::score, Scoring<2,0,1>::score},
		{Scoring<2,1,0>::score, Scoring<2,1,1>::score},
		{Scoring<2,2,0>::score, Scoring<2,2,1>::score},
		{Scoring<2,3,0>::score, Scoring<2,3,1>::score},
		{Scoring<2,4,0>::score, Scoring<2,4,1>::score},
	}, {
		{Scoring<3,0,0>::score, Scoring<3,0,1>::score},
		{Scoring<3,1,0>::score, Scoring<3,1,1>::score},
		{Scoring<3,2,0>::score, Scoring<3,2,1>::score},
		{Scoring<3,3,0>::score, Scoring<3,3,1>::score},
		{Scoring<3,4,0>::score, Scoring<3,4,1>::score},
	}, {
		{Scoring<4,0,0>::score, Scoring<4,0,1>::score},
		{Scoring<4,1,0>::score, Scoring<4,1,1>::score},
		{Scoring<4,2,0>::score, Scoring<4,2,1>::score},
		{Scoring<4,3,0>::score, Scoring<4,3,1>::score},
		{Scoring<4,4,0>::score, Scoring<4,4,1>::score},
	}, {
		{Scoring<5,0,0>::score, Scoring<5,0,1>::score},
		{Scoring<5,1,0>::score, Scoring<5,1,1>::score},
		{Scoring<5,3,0>::score, Scoring<5,2,1>::score},
		{Scoring<5,4,0>::score, Scoring<5,3,1>::score},
		{Scoring<5,4,0>::score, Scoring<5,4,1>::score},
	}
};

template<int d>
inline int AddAlignExpand(int & strictAlign, int &align, int &expand, bool & expandable, Goban const & g, unsigned int x, unsigned int y, int const p, Patterns::PatternInfos const * pInfos)
{
	ChangeCase<d>(x, y, pInfos->caseIndex);
	if (g.InBound(x, y) && (g[y][x] & Goban::PIONMASK) == p)
	{
		strictAlign += pInfos->align;
		align += pInfos->size - pInfos->expand - pInfos->align;
		expand += pInfos->expand;
		expandable = pInfos->expand;
		return pInfos->size + 1;
	}
	expand += pInfos->caseIndex - 1;
	expandable = pInfos->caseIndex - 1;
	return pInfos->caseIndex;
}

template<int d>
inline int GetThreatScore(Goban::PION_TYPE p, Goban const & g, unsigned int x, unsigned int y, int & nextEval, const Patterns::PatternInfos * pInfos1, const Patterns::PatternInfos * pInfos2)
{
	bool e1, e2;
	int align = 0;
	int expand = 0;
	int strictAlign = 0;
	nextEval = AddAlignExpand<d>(strictAlign, align, expand, e1, g, x, y, p, pInfos1);
	AddAlignExpand<d+4>(strictAlign, align, expand, e2, g, x, y, p, pInfos2);
	if (strictAlign + align + expand >= 4)
	{
		strictAlign = (strictAlign <= 4) ? strictAlign : 4;
		align = (align <= 4) ? align : 4;
		return Scores[strictAlign][align][e1 && e2];
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
void eval_case(int &score, Goban const &g, Goban::Case &toEval, Goban::PION_TYPE & currentPion, Goban::PION_TYPE &p)
{
	int x = 0, y = 0;
	XYManager<dir> manager;
	manager.init(x, y);
	int nextEval = 0;
	const Patterns::PatternInfos * pInfos1, * pInfos2;
	

	while (!manager.ended(x, y))
	{
		toEval = g[y][x];
		currentPion = (Goban::PION_TYPE)(toEval & Goban::PIONMASK);
		pInfos1 = Patterns::patterns + ((toEval >> Goban::HEADERSIZE >> Goban::PATTERNSIZE * dir) & Goban::PATTERNMASK);
		pInfos2 = Patterns::patterns + ((toEval >> Goban::HEADERSIZE >> Goban::PATTERNSIZE * (dir+4)) & Goban::PATTERNMASK);
		if (currentPion)
		{
			score += (currentPion == p) ? GetThreatScore<dir>(currentPion, g, x, y, nextEval, pInfos1, pInfos2) :
					-GetThreatScore<dir>(currentPion, g, x, y, nextEval, pInfos1, pInfos2);
			/*score += pow(5.0, Goban::GetInfluence(toEval, p)) * 2 ;
			score -= pow(5.0, Goban::GetInfluence(toEval, Goban::Other(p)));
			nextEval = 1;*/

		}
		else
			nextEval = (pInfos1->pattern) ? pInfos1->caseIndex : 5;
			//nextEval = 1;

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
