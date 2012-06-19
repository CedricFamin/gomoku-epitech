#include <limits>
#include <QDebug>
#include "alphabetathreading.h"

#include "PatternIdentifier.h"
#include "GobanIterator.h"

#include <iostream>
#include <sstream>

void print_goban(Goban & g)
{
	std::stringstream buffer;
    for (unsigned int y = 0; y < g.getHeight(); ++y)
	{
        for (unsigned int x = 0; x < g.getWidth(); ++x)
		{
			switch (g[y][x] & Goban::PIONMASK)
			{
			case Goban::BLACK:
				buffer << "o";
				break;
			case Goban::RED:
				buffer << "x";
				break;
			default:
				buffer << ".";
			}
		}
		buffer << std::endl;
	}
	qDebug() << buffer.str().c_str();
}

AlphaBetaThreading::AlphaBetaThreading(Goban & g, const Goban::Move & m, Goban::PION_TYPE p, Referrer & r)
    : _move(m), _pion(p), _goban(g), _score(0), _referrer(r)
{
	memset(this->_scoreTable, 0, sizeof(*_scoreTable) * 19 * 19);
}

int AlphaBetaThreading::_emptyPattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE)
{
	return 0;
}

int AlphaBetaThreading::__o_Pattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	return 0;
	unsigned int lx = GobanIterator::direction[dir][0] * 2 + x;
	unsigned int ly = GobanIterator::direction[dir][1] * 2 + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return 20;
	return 0;
}
int AlphaBetaThreading::__ooPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	return 0;
	unsigned int lx = GobanIterator::direction[dir][0] * 2 + x;
	unsigned int ly = GobanIterator::direction[dir][1] * 2 + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return 45;
	return -25;
}
int AlphaBetaThreading::__oo_Pattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	return 0;
	unsigned int lx = GobanIterator::direction[dir][0] * 2 + x;
	unsigned int ly = GobanIterator::direction[dir][1] * 2 + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return 30;
	return -10;
}
int AlphaBetaThreading::__ooxPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	return 0;
	unsigned int lx = GobanIterator::direction[dir][0] * 2 + x;
	unsigned int ly = GobanIterator::direction[dir][1] * 2 + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return -25;
	return 45;
}
int AlphaBetaThreading::__oooPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	return 0;
	unsigned int lx = GobanIterator::direction[dir][0] * 2 + x;
	unsigned int ly = GobanIterator::direction[dir][1] * 2 + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return 35;
	return -15;
}
int AlphaBetaThreading::_o_Pattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	unsigned int lx = GobanIterator::direction[dir][0] * 2 + x;
	unsigned int ly = GobanIterator::direction[dir][1] * 2 + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return 20;
	return 0;
}
int AlphaBetaThreading::_o_o_Pattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	return 0;
	unsigned int lx = GobanIterator::direction[dir][0] * 2 + x;
	unsigned int ly = GobanIterator::direction[dir][1] * 2 + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return 30;
	return -10;
}
int AlphaBetaThreading::_oxPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	return 0;
	unsigned int lx = GobanIterator::direction[dir][0] + x;
	unsigned int ly = GobanIterator::direction[dir][1] + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return -10;
	return 30;
}
int AlphaBetaThreading::_ooPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	return 0;
	unsigned int lx = GobanIterator::direction[dir][0] + x;
	unsigned int ly = GobanIterator::direction[dir][1] + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return 25;
	return 5;
}
 int AlphaBetaThreading::_oo_Pattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	unsigned int lx = GobanIterator::direction[dir][0] + x;
	unsigned int ly = GobanIterator::direction[dir][1] + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return 30;
	return -10;
}
int AlphaBetaThreading::_ooxPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	return 0;
	unsigned int lx = GobanIterator::direction[dir][0] + x;
	unsigned int ly = GobanIterator::direction[dir][1] + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return -15;
	return 35;
}
int AlphaBetaThreading::_oooPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	unsigned int lx = GobanIterator::direction[dir][0] + x;
	unsigned int ly = GobanIterator::direction[dir][1] + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return 40;
	return -20;
}
int AlphaBetaThreading::_ooo_Pattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	unsigned int lx = GobanIterator::direction[dir][0] + x;
	unsigned int ly = GobanIterator::direction[dir][1] + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return 50;
	return -30;
}
int AlphaBetaThreading::_oooxPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	unsigned int lx = GobanIterator::direction[dir][0] + x;
	unsigned int ly = GobanIterator::direction[dir][1] + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return 45;
	return -25;
}
int AlphaBetaThreading::_ooooPattern(Goban & g, unsigned int x, unsigned int y, int dir, Goban::PION_TYPE pion)
{
	
	unsigned int lx = GobanIterator::direction[dir][0] + x;
	unsigned int ly = GobanIterator::direction[dir][1] + y;
	if ((g[ly][lx] & Goban::PIONMASK) == pion)
		return 60;
	return -35;
}

int AlphaBetaThreading::eval(Goban & g, Goban::PION_TYPE pion)
{
    Goban::PION_TYPE currentPion;
    unsigned long long int current;
    int score = 0;
	int value;
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
	auto begin = this->_moveList.begin(), end = this->_moveList.end();
	for (; begin != end; ++begin)
	{
		for (int dist = 1; dist <= 4; dist++)
		{
			for (int dir = 0; dir < 8; ++dir)
			{
				unsigned int x = begin->first + GobanIterator::direction[dir][0] * dist;
				unsigned int y = begin->second + GobanIterator::direction[dir][1] * dist;
				if (!g.InBound(x, y))
					continue;
				current = g[y][x];
				value = this->_scoreTable[y * 19 + x];
				currentPion = (Goban::PION_TYPE)(current & Goban::PIONMASK);
				current >>= Goban::HEADERSIZE;
				if (!value ||1)
				{
					if (currentPion == 0 && current)
						for (unsigned int d = 0; d < 8; ++d, current >>= Goban::PATTERNSIZE)
						{
							int pattern = (current & Goban::PATTERNMASK);
							if (pattern == Patterns::oo_)
								value += (this->*scorings[current & Goban::PATTERNMASK])(g, x, y, d, pion);
						}
					this->_scoreTable[y * 19 + x] = value;
				}
				score += value;
			}
		}
    }
    return score;
}

std::list<Goban::Move> AlphaBetaThreading::GetTurns(Goban & g,Goban::Move & last , Goban::PION_TYPE)
{
    std::list<Goban::Move> possiblesTurns;
	for (unsigned int x = 0; x < g.getWidth(); ++x) 
        for (unsigned int y = 0; y < g.getHeight(); ++y)
			if (!(g[y][x] & Goban::PIONMASK) && ((g[y][x] & ~Goban::PIONMASK)))
				possiblesTurns.push_back(std::make_pair(x, y));
    return possiblesTurns;
}

void AlphaBetaThreading::run()
{
	if (this->_referrer(this->_goban, this->_pion, this->_move.first, this->_move.second))
	{
		print_goban(this->_goban);
		this->_moveList.push_front(std::make_pair(this->_move.first , this->_move.second));
		this->eval(this->_goban, this->_pion);
		this->_score = this->alphabeta(this->_goban, 0,
			                           std::numeric_limits<int>::min() + 1, std::numeric_limits<int>::max(),
				                       (this->_pion == Goban::BLACK) ? Goban::RED: Goban::BLACK);
	}
	else
		this->_score = -std::numeric_limits<int>::max();
}

void AlphaBetaThreading::update(Goban & g, int x, int y)
{
	for (int dist = 1; dist <= 4; dist++)
	{
		for (int dir = 0; dir < 8; ++dir)
		{
			unsigned int lx = x + GobanIterator::direction[dir][0] * dist;
			unsigned int ly = y + GobanIterator::direction[dir][1] * dist;
			if (g.InBound(lx, ly))
				this->_scoreTable[ly * 19 + lx] = 0;
		}
	}
}

int AlphaBetaThreading::alphabeta(Goban & g, int depth, int alpha, int beta, Goban::PION_TYPE pion)
{
	if (g.gameFinished())
	{
		return g.getWinner() == pion ? std::numeric_limits<int>::max() : -std::numeric_limits<int>::max();
	}
	qDebug() << "text";
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
			if (this->_referrer(g, pion, x, y))
			{
				update(g, x, y);
				this->_moveList.push_front(std::make_pair(x , y));
				value = -alphabeta(s, depth - 1, -beta, -alpha, (pion == Goban::BLACK) ? Goban::RED: Goban::BLACK);
				update(g, x, y);
				this->_moveList.pop_front();
				if (value > best)
				{
					best = value;
					if (best > alpha)
					{
						alpha = best;
						if (alpha > beta) return best;
					}
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
