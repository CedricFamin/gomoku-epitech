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
	~AlphaBetaThreading();
    int getScore() const;
    Goban::Move const & getMove() const;
	static std::list<Goban::Move> GetTurns(Goban & g);
protected:
    virtual void run();
private:
	static std::list<AlphaBetaThreading*> _idleThread;
	static unsigned int _activeThread;

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
		bool isAlpha = pion == this->_pion;

		if (g.gameFinished())
			return g.getWinner() == pion ? std::numeric_limits<int>::max() : -std::numeric_limits<int>::max();
		for (unsigned int x = 0,y = 0; y < 19  && beta > alpha;)
		{
			alpha = std::max(alpha, AlphaBetaThreading::GlobalAlpha);
			if (alpha >= beta) break;
			toEval = g[y][x];
			if ((toEval & ~Goban::PIONMASK))
			{
				toEval >>= Goban::HEADERSIZE;
				Goban s = g;
				for (int i = 0; i < 8; ++i)
				{
					const Patterns::PatternInfos * p = Patterns::patterns + (toEval & Goban::PATTERNMASK);
					if (p->caseIndex <= 2 && p->pattern && this->_referrer(s, pion, x, y))
					{
						if (isAlpha)
							alpha = std::max(alpha, alphabeta<depth - 1>(s, alpha, beta, Goban::Other(pion)));
						else
							beta = std::min(beta, alphabeta<depth - 1>(s, alpha, beta, Goban::Other(pion)));
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
		return isAlpha ? alpha : beta;
	}
};

#endif // ALPHABETATHREADING_H
