#ifndef ALPHABETATHREADING_H
#define ALPHABETATHREADING_H

#include <QThread>
#include "Goban.h"
#include "Referrer.h"
#include "Evaluator.h"

class AlphaBetaThreading : public QThread
{
public:
	static int GlobalAlpha;
    AlphaBetaThreading(Goban &, Goban::Move const &, Goban::PION_TYPE, Referrer & r);
    int getScore() const;
    Goban::Move const & getMove() const;
	static std::list<Goban::Move> GetTurns(Goban & g,Goban::Move & last , Goban::PION_TYPE);
protected:
    virtual void run();
private:
    int alphabeta(Goban &g, int depth, int alpha, int beta, Goban::PION_TYPE pion);
	
	void update(Goban & g, int x, int y);

	bool _toEval[19*19];
    Goban::Move _move;
    Goban::PION_TYPE _pion;
    Goban _goban;
    int _score;
	int _scoreTable[19*19];
    std::list<Goban::Move> _moveList;
	Referrer _referrer;
	Evaluator _evaluator;

};

#endif // ALPHABETATHREADING_H
