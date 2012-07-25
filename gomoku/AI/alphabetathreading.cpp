#include <limits>
#include "alphabetathreading.h"

#include "PatternIdentifier.h"
#include "GobanIterator.h"

int AlphaBetaThreading::GlobalAlpha = 0;
std::list<AlphaBetaThreading*> AlphaBetaThreading::_idleThread;
unsigned int AlphaBetaThreading::_activeThread = 0;

AlphaBetaThreading::AlphaBetaThreading(Goban & g, const Goban::Move & m, Goban::PION_TYPE p, Referrer & r)
  : _move(m), _pion(p), _goban(g), _score(0), _referrer(r), _evaluator(&g)
{
	if (_activeThread <= QThread::idealThreadCount())
	{
		++_activeThread;
		this->start();
	}
	else _idleThread.push_back(this);
}

AlphaBetaThreading::~AlphaBetaThreading()
{
	if (_idleThread.size())
	{
		_idleThread.front()->start();
		_idleThread.pop_front();
	}
	else _activeThread--;
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
		  this->_score = this->alphabeta<3>(s,
					-std::numeric_limits<int>::max(), std::numeric_limits<int>::max(),
					Goban::Other(this->_pion));
  }
  else
    this->_score = std::numeric_limits<int>::min();
}

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
