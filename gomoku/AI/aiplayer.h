#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "iplayer.h"

#include "goban.h"
#include "Referrer.h"

class AIPlayer : public IPlayer
{
public:
    typedef std::pair<unsigned int, unsigned int> Move;
    AIPlayer(Goban::PION_TYPE);
    virtual Goban::PION_TYPE getColor() const;
    virtual void play(Referrer & r, callback_type);
    Move alphabeta(Move & last, Goban & g, Referrer & r);
private:
    Goban::PION_TYPE _color;
};

#endif // AIPLAYER_H
