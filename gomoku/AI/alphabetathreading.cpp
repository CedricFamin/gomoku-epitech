#include <limits>
#include <QDebug>
#include "alphabetathreading.h"

#include "PatternIdentifier.h"
#include "GobanIterator.h"

#include <iostream>
#include <sstream>
#include <queue>

int AlphaBetaThreading::GlobalAlpha = 0;

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
  : _move(m), _pion(p), _goban(g), _score(0), _referrer(r), _evaluator(&g)
{
  //memset(this->_scoreTable, 0, sizeof(*_scoreTable) * 19 * 19);
}

std::list<Goban::Move> AlphaBetaThreading::GetTurns(Goban & g,Goban::Move & last , Goban::PION_TYPE)
{
  std::list<Goban::Move> possiblesTurns;
  Goban::Case toEval;

  for (unsigned int x = 0; x < g.getWidth(); ++x) 
    for (unsigned int y = 0; y < g.getHeight(); ++y)
	{
		toEval = g[y][x];
		if (!(toEval & Goban::PIONMASK))
		{
			toEval >>= Goban::HEADERSIZE;
			for (int i = 0; i < 8; ++i)
			{
				const Patterns::PatternInfos * p = Patterns::patterns + (toEval & Goban::PATTERNMASK);
				if (p->caseIndex <= 2 && p->pattern)
				{
					possiblesTurns.push_back(std::make_pair(x, y));
					i = 8;
				}
				toEval >>= Goban::PATTERNSIZE;
			}
		}
	}
  return possiblesTurns;
}

void AlphaBetaThreading::run()
{
  Goban s = this->_goban;
  if (this->_referrer(s, this->_pion, this->_move.first, this->_move.second))
    {
      this->_score = this->alphabeta(s, 3,
				      std::numeric_limits<int>::min() + 1, std::numeric_limits<int>::max(),
				      Goban::Other(this->_pion));
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

struct InfluenceCompare
{
	bool operator()(std::pair<int, Goban::Move> const & i1, std::pair<int, Goban::Move> const & i2)
	{
		return i1.first > i2.first;
	}
};

int AlphaBetaThreading::alphabeta(Goban & g, int depth, int alpha, int beta, Goban::PION_TYPE pion)
{
	if (g.gameFinished()) return g.getWinner() == pion ? std::numeric_limits<int>::max() : -std::numeric_limits<int>::max();
	if (depth == 0) return this->_evaluator(g,pion);

	Goban::Case toEval;

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
						alpha = std::max(alpha, alphabeta(g, depth - 1, alpha, beta, Goban::Other(pion)));
					else
						beta = std::min(beta, alphabeta(g, depth - 1, alpha, beta, Goban::Other(pion)));
					g.subIn(x, y, false);
					i = 8;
				}
				toEval >>= Goban::PATTERNSIZE;
			}
			/*if (this->_referrer(g, pion, x, y, false))
			{
				if (pion == this->_pion)
					alpha = std::max(alpha, alphabeta(g, depth - 1, alpha, beta, Goban::Other(pion)));
				else
					beta = std::min(beta, alphabeta(g, depth - 1, alpha, beta, Goban::Other(pion)));
				g.subIn(x, y, false);
			}*/
		}
		if (++x >= 19)
		{
			x = 0;
			y++;
		}
	}
	return pion == this->_pion ? alpha : beta;
}

Goban::Move const & AlphaBetaThreading::getMove() const
{
  return this->_move;
}

int AlphaBetaThreading::getScore() const
{
  return this->_score;
}
