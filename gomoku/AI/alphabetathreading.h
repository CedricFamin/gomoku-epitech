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
	static int sun;
	static int GlobalAlpha;
    AlphaBetaThreading(Goban &, Goban::Move const &, Goban::PION_TYPE, Referrer & r);
	~AlphaBetaThreading();
    int getScore() const;
    Goban::Move const & getMove() const;
	static std::list<Goban::Move> GetTurns(Goban & g);
	struct Turns
	{
		inline Turns() {}
		inline Turns(unsigned int _x, unsigned int _y, int i) : x(_x), y(_y), influence(i) { }
		unsigned int x;
		unsigned int y;
		int influence;
	};

	static inline int GetMoves(Goban const & g, Turns turns[361])
	{
		int nbTurns = 0;
		Goban::Case toEval;
		int influence;
		for (unsigned int x = 0,y = 0; y < 19;)
		{
			toEval = g[y][x];
			if ((toEval & ~Goban::PIONMASK) && (toEval & Goban::PIONMASK) == 0)
			{
				influence = Goban::GetInfluence(toEval, Goban::BLACK) + Goban::GetInfluence(toEval, Goban::RED);
				toEval >>= Goban::HEADERSIZE;	
				for (int i = 0; i < 8; ++i)
				{
					const Patterns::PatternInfos * p = Patterns::patterns + (toEval & Goban::PATTERNMASK);
					if (p->caseIndex <= 2 && p->pattern)
					{
						turns[nbTurns++] = Turns(x, y, influence);
						for(Turns * t = turns + nbTurns - 2; t >= turns; --t)
						{
							if (t->influence < (t+1)->influence)
							{
								(t+1)->x = t->x;
								(t+1)->y = t->y;
								(t+1)->influence = t->influence;
								t->x = x;
								t->y = y;
								t->influence = influence;
							}
							else break;
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
		return nbTurns;
	}
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
		bool isAlpha = pion == this->_pion;
		int nbTurns = 0;
		int i = 0;
		Turns turns[361];

		if (g.gameFinished())
			return this->_evaluator(g, pion);
			//return g.getWinner() == pion ? std::numeric_limits<int>::max() : -std::numeric_limits<int>::max();
		nbTurns = this->GetMoves(g, turns);
		for (Turns * t = turns; t < turns + nbTurns && i < 40 && beta > alpha; ++t)
		{
			alpha = std::max(alpha, AlphaBetaThreading::GlobalAlpha);
			if (alpha >= beta) break;
			Goban s = g;
			if (this->_referrer(s, pion, t->x, t->y))
			{
				++i;
				if (isAlpha)
					alpha = std::max(alpha, alphabeta<depth - 1>(s, alpha, beta, Goban::Other(pion)));
				else
					beta = std::min(beta, alphabeta<depth - 1>(s, alpha, beta, Goban::Other(pion)));
			}
		}
		return isAlpha ? alpha : beta;
	}
};

#endif // ALPHABETATHREADING_H
