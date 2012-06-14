#include <limits>
#include <QDebug>
#include "alphabetathreading.h"

#include "PatternIdentifier.h"
#include "GobanIterator.h"

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


int AlphaBetaThreading::_emptyPattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE)
{
	return 0;
}

int AlphaBetaThreading::__o_Pattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	return 0;
}
int AlphaBetaThreading::__ooPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	return 0;
}
int AlphaBetaThreading::__oo_Pattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	return 0;
}
int AlphaBetaThreading::__ooxPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	return 0;
}
int AlphaBetaThreading::__oooPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	return 0;
}
int AlphaBetaThreading::_o_Pattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	return 0;
}
int AlphaBetaThreading::_o_o_Pattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	return 0;
}
int AlphaBetaThreading::_oxPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	GobanIterator it(g, x, y);
	if ((it[dir] & Goban::PIONMASK) == pion)
		return -10;
	return 10;
}
int AlphaBetaThreading::_ooPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	return 0;
}
 int AlphaBetaThreading::_oo_Pattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	GobanIterator it(g, x, y);
	if ((it[dir] & Goban::PIONMASK) == pion)
		return 10;
	return 0;
}
int AlphaBetaThreading::_ooxPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	GobanIterator it(g, x, y);
	if ((it[dir] & Goban::PIONMASK) == pion)
		return 0;
	return 00;
}
int AlphaBetaThreading::_oooPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	GobanIterator it(g, x, y);
	if ((it[dir] & Goban::PIONMASK) == pion)
		return 00;
	return 0;
}
int AlphaBetaThreading::_ooo_Pattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	GobanIterator it(g, x, y);
	if ((it[dir] & Goban::PIONMASK) == pion)
		return 00;
	return 0;
}
int AlphaBetaThreading::_oooxPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	GobanIterator it(g, x, y);
	if ((it[dir] & Goban::PIONMASK) == pion)
		return 0;
	return 0;
}
int AlphaBetaThreading::_ooooPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	GobanIterator it(g, x, y);
	if ((it[dir] & Goban::PIONMASK) == pion)
		return 0;
	return 0;
}

int AlphaBetaThreading::eval(Goban & g, Goban::PION_TYPE pion)
{
    Goban::PION_TYPE currentPion;
    unsigned long long int current;
    int score = 0, value;
	typedef int (AlphaBetaThreading::*ScoringMethod)(Goban &, unsigned int, unsigned int, int, Goban::PION_TYPE);
	ScoringMethod scorings[] = {
		&AlphaBetaThreading::_emptyPattern,
		&AlphaBetaThreading::__o_Pattern,
		&AlphaBetaThreading::__ooPattern,
		&AlphaBetaThreading::__oo_Pattern,
		&AlphaBetaThreading::__ooxPattern,
		&AlphaBetaThreading::__oooPattern,
		&AlphaBetaThreading::_o_Pattern,
		&AlphaBetaThreading::_o_o_Pattern,
		&AlphaBetaThreading::_oxPattern,
		&AlphaBetaThreading::_ooPattern,
		&AlphaBetaThreading::_oo_Pattern,
		&AlphaBetaThreading::_ooxPattern,
		&AlphaBetaThreading::_oooPattern,
		&AlphaBetaThreading::_ooo_Pattern,
		&AlphaBetaThreading::_oooxPattern,
		&AlphaBetaThreading::_ooooPattern,
	};

    for (unsigned int x = 0; x < g.getWidth(); ++x)
    {
        for (unsigned int y = 0; y < g.getHeight(); ++y)
        {
            current = g[y][x];
            value = 0;
            currentPion = (Goban::PION_TYPE)(current & Goban::PIONMASK);
            if (currentPion != 0)
                continue;
            current >>= Goban::HEADERSIZE;
            if (!current)
                continue;
            for (unsigned int d = 0; d < 8; ++d)
            {
				if ((current & Goban::PATTERNMASK) <= Patterns::oooo)
				{
					score += (this->*scorings[current & Goban::PATTERNMASK])(g, x, y, d, pion);
				}
                current >>= Goban::PATTERNSIZE;
            }
        }
    }
    return score;
}

std::list<Goban::Move> AlphaBetaThreading::GetTurns(Goban & g,Goban::Move & last , Goban::PION_TYPE)
{
    std::list<Goban::Move> possiblesTurns;
	for (unsigned int x = 0; x < g.getWidth(); ++x)
    {
        for (unsigned int y = 0; y < g.getHeight(); ++y)
        {
			if (!(g[y][x] & Goban::PIONMASK) && ((g[y][x] & ~Goban::PIONMASK) || (abs((int)x - (int)last.first) <= 1 && abs((int)y - (int)last.second) <= 1)))
			{
				possiblesTurns.push_back(std::make_pair(x, y));
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
    this->_goban.Putin(this->_pion, this->_move.first, this->_move.second);
    this->_moveList = GetTurns(this->_goban, this->_move, this->_pion);
    this->_score = this->alphabeta(this->_move, this->_goban, 1,
                                   std::numeric_limits<int>::min() + 1, std::numeric_limits<int>::max(),
                                   (this->_pion == Goban::BLACK) ? Goban::RED: Goban::BLACK);
	qDebug() << this->_score;
}

int AlphaBetaThreading::alphabeta(Goban::Move &, Goban & g, int depth, int alpha, int beta, Goban::PION_TYPE pion)
{
    static int i = 0;
    i++;
    if (depth == 0)
    {
        return eval(g, pion);
    }
    int best = std::numeric_limits<int>::min() + 1;
    for (unsigned int x = 0; x < g.getWidth(); ++x)
    {
        for (unsigned int y = 0; y < g.getHeight(); ++y)
        {
			if (!((g[y][x] & ~Goban::PIONMASK))) continue;
            int value = 0;
            Goban s = g;
            s.Putin(pion, x, y);
            value = -alphabeta(this->_move, s, depth - 1, -beta, -alpha, (pion == Goban::BLACK) ? Goban::RED: Goban::BLACK);
            if (value > best)
            {
                best = value;
                if (best > alpha)
                {
                    alpha = best;
                    if (alpha > beta) 
						return best;
                }
            }
        }
    }
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
