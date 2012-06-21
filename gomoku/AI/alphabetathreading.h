#ifndef ALPHABETATHREADING_H
#define ALPHABETATHREADING_H

#include <QThread>
#include "Goban.h"
#include "Referrer.h"
#include "Evaluator.h"

class AlphaBetaThreading : public QThread
{
public:
    AlphaBetaThreading(Goban &, Goban::Move const &, Goban::PION_TYPE, Referrer & r);
    int getScore() const;
    Goban::Move const & getMove() const;
	int eval(Goban & g, Goban::PION_TYPE pion);
	static std::list<Goban::Move> GetTurns(Goban & g,Goban::Move & last , Goban::PION_TYPE);
protected:
    virtual void run();
private:
    int alphabeta(Goban &g, int depth, int alpha, int beta, Goban::PION_TYPE pion);

	int __o_Pattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int __ooPattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int __oo_Pattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int __ooxPattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int __oooPattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int _o_Pattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int _o_o_Pattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int _oxPattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int _ooPattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int _oo_Pattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int _ooxPattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int _oooPattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int _ooo_Pattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int _oooxPattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int _ooooPattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int _emptyPattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	
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
