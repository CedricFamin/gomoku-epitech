#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "iplayer.h"

#include "goban.h"
#include "Referrer.h"

class AIPlayer : public IPlayer
{
public:
    AIPlayer(Goban::PION_TYPE);
    virtual Goban::PION_TYPE getColor() const;
    virtual void play(Referrer & r, callback_type);
private:
    std::list<std::pair<unsigned int, unsigned int> > _getTurns(Goban & g, Referrer & r ,Goban::PION_TYPE pion);
    Goban::PION_TYPE _color;
};

#endif // AIPLAYER_H
