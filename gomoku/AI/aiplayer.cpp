#include <algorithm>
#include <limits>
#include <iostream>

#include <time.h>
#include <QDebug>
#include "GobanIterator.h"
#include "alphabetathreading.h"

#include "aiplayer.h"

AIPlayer::AIPlayer(Goban::PION_TYPE pion) : _color(pion)
{

}

Goban::PION_TYPE AIPlayer::getColor() const
{
  return this->_color;
}

Goban::Move AIPlayer::play(Referrer & r, Goban & g)
{	Evaluator eval(&g);
  Move move;
  if (g.nbTurn == 0)
    {
      move.first = 9;
      move.second = 9;
    }
  else move = this->alphabeta(g, r);
  if (r(g, this->getColor(), move.first, move.second))
    {
      return move;
    }
  return Goban::Move(-1, -1);
}

AIPlayer::Move AIPlayer::alphabeta(Goban & g, Referrer & r)
{

    Move bestMove;
	int bestScore = -std::numeric_limits<int>::max(), score = std::numeric_limits<int>::min();
	AlphaBetaThreading::GlobalAlpha = bestScore;
	std::list<Move> turns = AlphaBetaThreading::GetTurns(g);
    std::list<AlphaBetaThreading*> workers;
    std::for_each(turns.begin(), turns.end(),
    [&g, &workers, this, &r](Move & m)
    {
        workers.push_back(new AlphaBetaThreading(g, m, this->_color, r));
        workers.back()->start();
    });
    std::for_each(workers.begin(), workers.end(),
    [&bestMove, &bestScore, &score](AlphaBetaThreading* worker)
    {
        worker->wait();
        score = worker->getScore();
        if (score > bestScore)
        {
			AlphaBetaThreading::GlobalAlpha = score;
            bestScore = score;
            bestMove.first = worker->getMove().first;
            bestMove.second = worker->getMove().second;
        }
        delete worker;
    });
    return bestMove;
}
