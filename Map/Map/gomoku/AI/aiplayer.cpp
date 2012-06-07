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

void AIPlayer::play(Referrer & r, callback_type callback)
{
    Move move;
    if (r.GetListOfTurn().size() == 0)
    {
        move.first = 9;
        move.second = 9;
    }
    else
    {
        move.first = r.GetListOfTurn().back().x;
        move.second = r.GetListOfTurn().back().y;
        move = this->alphabeta(move, r.getGoban());
    }
    if (r.CanPlay(this->getColor(), move.first, move.second))
    {
        r.Play();
        r.AfterPlay();
        callback(this->getColor(), move.first, move.second);
    }
}

AIPlayer::Move AIPlayer::alphabeta(Move & last, Goban & g)
{
    Move bestMove;
    int bestScore = std::numeric_limits<int>::min();
    std::list<Move> turns = _getTurns(g, last, this->_color);
    std::list<AlphaBetaThreading*> workers;
    std::for_each(turns.begin(), turns.end(),
    [&g, &workers, this](Move & m)
    {
        workers.push_back(new AlphaBetaThreading(g, m, this->_color));
        workers.back()->start();
    });
qDebug() << workers.size();
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

std::list<AIPlayer::Move> AIPlayer::_getTurns(Goban & g, Move & last ,Goban::PION_TYPE)
{
    const int moves[4][2] = {
            { 1, 0}, { 0, 1}, {-1, 0}, {0, -1}
        };

    std::list<Move> possiblesTurns;
    int width = 0;
    int lx = last.first;
    int ly = last.second;

    for (int i = 0; i < 2; ++i)
    {
        width += 2;
        lx -= 1;
        ly -= 1;
        for (int j = 0; j < 4; ++j)
        {
            for (int y = 0; y < width; ++y)
            {
                lx += moves[j][0];
                ly += moves[j][1];
                if (g.InBound(lx, ly) && (g.GetMap()[ly][lx] & Goban::PIONMASK) == 0)
                    possiblesTurns.push_back(std::make_pair(lx, ly));
            }
        }
    }
    return possiblesTurns;
}
