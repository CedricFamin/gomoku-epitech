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

std::list<Goban::Move> AlphaBetaThreading::GetTurns(Goban & g)
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
	  if (s.gameFinished())
		  this->_score = (s.getWinner() == this->_pion) ? std::numeric_limits<int>::max() : -std::numeric_limits<int>::max();
	  else
		  this->_score = this->alphabeta<1>(s,
					-std::numeric_limits<int>::max(), std::numeric_limits<int>::max(),
					Goban::Other(this->_pion));
  }
  else
    this->_score = std::numeric_limits<int>::min();
}

struct InfluenceCompare
{
	bool operator()(std::pair<int, Goban::Move> const & i1, std::pair<int, Goban::Move> const & i2)
	{
		return i1.first > i2.first;
	}
};

Goban::Move const & AlphaBetaThreading::getMove() const
{
  return this->_move;
}

int AlphaBetaThreading::getScore() const
{
  return this->_score;
}

template<>
int AlphaBetaThreading::alphabeta<0>(Goban & g, int, int, Goban::PION_TYPE pion)
{
	return this->_evaluator(g,pion);
}