#ifndef ALPHABETATHREADING_H
#define ALPHABETATHREADING_H

#include <QThread>
#include "Goban.h"
#include "Referrer.h"
#include "Evaluator.h"
#include "PatternIdentifier.h"

class AlphaBetaThreading : public QThread
{
public:
	static int GlobalAlpha;
    AlphaBetaThreading(Goban &, Goban::Move const &, Goban::PION_TYPE, Referrer & r);
    int getScore() const;
    Goban::Move const & getMove() const;
	static std::list<Goban::Move> GetTurns(Goban & g);
protected:
    virtual void run();
private:
    Goban::Move _move;
    Goban::PION_TYPE _pion;
    Goban _goban;
    int _score;
	Referrer _referrer;
	Evaluator _evaluator;

	template<int depth>
	int alphabeta(Goban & g, int alpha, int beta, Goban::PION_TYPE pion)
	{
		Goban::Case toEval;

		if (g.gameFinished())
		{
			return g.getWinner() == this->_pion ? std::numeric_limits<int>::max() : -std::numeric_limits<int>::max();
		}
		for (unsigned int x = 0,y = 0; y < 19  && beta > alpha; ++x)
		{
			alpha = std::max(alpha, AlphaBetaThreading::GlobalAlpha);
			if (alpha >= beta) break;
			toEval = g[y][x];
			if ((toEval & ~Goban::PIONMASK))
			{
				toEval >>= Goban::HEADERSIZE;
				for (int i = 0; i < 8; ++i)
				{
					const Patterns::PatternInfos * p = Patterns::patterns + (toEval & Goban::PATTERNMASK);
					if (p->caseIndex <= 2 && p->pattern && this->_referrer(g, pion, x, y, false))
					{
						if (pion == this->_pion)
							alpha = std::max(alpha, alphabeta<depth - 1>(g, alpha, beta, Goban::Other(pion)));
						else
							beta = std::min(beta, alphabeta<depth - 1>(g, alpha, beta, Goban::Other(pion)));
						g.subIn(x, y, false);
						g.setGameFinished(false);
						while (g.toDelete.size())
						{
							g.Putin(Goban::Other(pion), g.toDelete.top().first, g.toDelete.top().second);
							g.toDelete.pop();
							--(g.setDeletedStone(Goban::Other(pion)));
						}
						i = 8;
					}
					toEval >>= Goban::PATTERNSIZE;
				}
			}
			if (++x >= 19)
			{
				x = 0;
				y++;
			}
		}
		return pion == this->_pion ? alpha : beta;
	}
};

#endif // ALPHABETATHREADING_H
