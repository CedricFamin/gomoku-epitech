#include <algorithm>
#include <limits>
#include <iostream>

#include <time.h>
#include <QDebug>
#include "alphabetathreading.h"

#include "aiplayer.h"

AIPlayer::AIPlayer(Goban::PION_TYPE pion) : _color(pion)
{

}

Goban::PION_TYPE AIPlayer::getColor() const
{
    return this->_color;
}

void AIPlayer::play(Referrer & r, Goban & g, callback_type callback)
{
    Move move;
    if (g.Turns().size() == 0)
    {
        move.first = 9;
        move.second = 9;
    }
    else
    {
        move.first = g.Turns().back().x;
        move.second = g.Turns().back().y;
        move = this->alphabeta(move, g, r);
    }
    if (r(g, this->getColor(), move.first, move.second))
    {
        callback(this->getColor(), move.first, move.second);
    }
}

AIPlayer::Move AIPlayer::alphabeta(Move & last, Goban & g, Referrer & r)
{
    Move bestMove;
    int bestScore = std::numeric_limits<int>::min();
	std::list<Move> turns = AlphaBetaThreading::GetTurns(g, last, this->_color);
    std::list<AlphaBetaThreading*> workers;
    std::for_each(turns.begin(), turns.end(),
    [&g, &workers, this, &r](Move & m)
    {
        workers.push_back(new AlphaBetaThreading(g, m, this->_color, r));
        workers.back()->start();
    });
    std::for_each(workers.begin(), workers.end(),
    [&bestMove, &bestScore](AlphaBetaThreading* worker)
    {
        int score;
        worker->wait();
        score = worker->getScore();
        if (score > bestScore)
        {
            bestScore = score;
            bestMove.first = worker->getMove().first;
            bestMove.second = worker->getMove().second;
        }
        delete worker;
    });
    return bestMove;
}