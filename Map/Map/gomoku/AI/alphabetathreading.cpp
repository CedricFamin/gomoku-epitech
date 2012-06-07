#include <limits>
#include "alphabetathreading.h"

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
            if (currentPion == 0)
                continue;
            current >>= Goban::HEADERSIZE;
            for (unsigned int d = 0; d < 8; ++d)
            {
                //if ((current & Goban::PIONMASK) == currentPion)
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
            if (current == pion)
                score += value;
            else
                score -= value;
        }
    }
    return score;
}

std::list<Goban::Move> _getTurns(Goban & g,Goban::Move & last , Goban::PION_TYPE)
{
    const int moves[4][2] = {
            { 1, 0}, { 0, 1}, {-1, 0}, {0, -1}
        };

    std::list<Goban::Move> possiblesTurns;
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


AlphaBetaThreading::AlphaBetaThreading(Goban & g, const Goban::Move & m, Goban::PION_TYPE p)
    : _move(m), _pion(p), _goban(g), _score(0)
{
}

void AlphaBetaThreading::run()
{
    this->_score = this->alphabeta(this->_move, this->_goban, 3,
                            std::numeric_limits<int>::min() + 1, std::numeric_limits<int>::max(), this->_pion);
}

int AlphaBetaThreading::alphabeta(Goban::Move & last, Goban & g, int depth, int alpha, int beta, Goban::PION_TYPE pion)
{
    if (depth == 0)
        return eval(g, pion);
    bool stop = false;
    int best = std::numeric_limits<int>::min() + 1;
    std::list<Goban::Move> turns = _getTurns(g, last, pion);
    std::for_each(turns.begin(), turns.end(),
    [this, &best, &g, depth, &alpha, &beta, pion, &stop](Goban::Move & coord)
    {
        if (stop)
           return ;
        int value;
        g.Putin(pion, coord.first, coord.second);
        value = -alphabeta(coord, g, depth - 1, -beta, -alpha, (pion == Goban::BLACK) ? Goban::RED: Goban::BLACK);
        g.subIn(coord.first, coord.second);
        if (value > best)
        {
            best = value;
            if (best > alpha)
            {
                alpha = best;
                if (alpha > beta) stop = true;
            }
        }
    });
    return best;
}

Goban::Move const & AlphaBetaThreading::getMove() const
{
    return this->_move;
}

int AlphaBetaThreading::getScore() const
{
    return this->_score;
}
