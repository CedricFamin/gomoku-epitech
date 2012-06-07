#ifndef ALPHABETATHREADING_H
#define ALPHABETATHREADING_H

#include <QThread>
#include "Goban.h"

class AlphaBetaThreading : public QThread
{
public:
    AlphaBetaThreading(Goban &, Goban::Move const &, Goban::PION_TYPE);
    int getScore() const;
    Goban::Move const & getMove() const;
protected:
    virtual void run();
private:
    int alphabeta(Goban::Move &last, Goban &g, int depth, int alpha, int beta, Goban::PION_TYPE pion);
    Goban::Move _move;
    Goban::PION_TYPE _pion;
    Goban _goban;
    int _score;

};

#endif // ALPHABETATHREADING_H
