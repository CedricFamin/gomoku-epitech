#ifndef IPLAYER_H
#define IPLAYER_H

#include "goban.h"
#include "Referrer.h"

class IPlayer
{
public:
    virtual Goban::PION_TYPE getColor() const = 0;
    virtual void play(Referrer & r) = 0;
};

#endif // IPLAYER_H
