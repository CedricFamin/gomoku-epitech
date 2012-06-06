#include <algorithm>
#include <limits>
#include <iostream>

#include <time.h>
#include <QDebug>

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
        this->alphabeta(move, r.getGoban(), 0, std::numeric_limits<int>::min() + 1, std::numeric_limits<int>::max(), this->_color, &move);
    }
    if (r.CanPlay(this->getColor(), move.first, move.second))
    {
        r.Play();
        r.AfterPlay();
        callback(this->getColor(), move.first, move.second);
    }
}

int countBit(unsigned long long int value, int max)
{
    int count = 0;
    for (int i = 0; i < max; ++i)
    {
        count += value & 1;
        value >>= 1;
    }
    return count;
}

int eval(Goban & g, Goban::PION_TYPE pion)
{
    Goban::PION_TYPE currentPion;
    Goban::Case ** map = g.GetMap();
    unsigned long long int current;
    int score = 0, value;
    for (unsigned int x = 0; x < g.getWidth(); ++x)
    {
        for (unsigned int y = 0; y < g.getHeight(); ++y)
        {
            current = map[y][x];
            value = 0;
            currentPion = (Goban::PION_TYPE)(current & Goban::PIONMASK);
            if (currentPion != pion)
                continue;
            current >>= Goban::HEADERSIZE;
            for (unsigned int d = 0; d < 8; ++d)
            {
                if ((current & Goban::PIONMASK) == currentPion)
                {
                    switch (countBit(current >> Goban::COLORSIZE >> 1, 4))
                    {
                    case 4:  value += 810;
                    case 3:  value += 81;
                    case 2:  value += 8;
                    case 1:  value += 1;
                    default: value += 0;
                    }
                }
                current >>= Goban::PATTERNSIZE;
            }
            score += value;
        }
    }
    return score;
}

int AIPlayer::alphabeta(Move & last, Goban & g, unsigned int depth, int alpha, int beta, Goban::PION_TYPE pion, Move * move = 0)
{
    if (depth == 2)
    {
        return eval(g, pion);
    }
    bool stop = false;
    int best = std::numeric_limits<int>::min() + 1;
    std::list<Move> turns = _getTurns(g, last, pion);
    std::for_each(turns.begin(), turns.end(),
    [this, &best, &g, depth, &alpha, &beta, pion, &stop, &move](Move & coord)
    {
        if (stop)
           return ;
        Goban sun = g;
        int value;

        sun.Putin(pion, coord.first, coord.second);
        value = -alphabeta(coord, sun, depth + 1, -beta, -alpha, (pion == Goban::BLACK) ? Goban::RED: Goban::BLACK);
        if (value > best)
        {
            best = value;
            if (best > alpha)
            {
                if (move)
                {
                    move->first = coord.first;
                    move->second = coord.second;
                }
                alpha = best;
                if (alpha > beta)
                {
                    stop = true;
                }
            }
        }
    });
    return best;
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
