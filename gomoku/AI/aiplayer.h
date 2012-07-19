#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "iplayer.h"

#include "Goban.h"
#include "Referrer.h"

class AIPlayer : public IPlayer
{
public:
    typedef std::pair<unsigned int, unsigned int> Move;
    AIPlayer(Goban::PION_TYPE);
    virtual Goban::PION_TYPE getColor() const;
    virtual Goban::Move play(Referrer & r, Goban & g);
    Move alphabeta(Goban & g, Referrer & r);
private:
    Goban::PION_TYPE _color;
};

#endif // AIPLAYER_H
