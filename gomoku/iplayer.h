#ifndef IPLAYER_H
#define IPLAYER_H

#include "Goban.h"
#include "Referrer.h"

class IPlayer
{
public:
    typedef std::tr1::function<void (Goban::PION_TYPE, int, int)> callback_type;
    virtual Goban::PION_TYPE getColor() const = 0;
    virtual Goban::Move play(Referrer & r, Goban & g) = 0;
};

#endif // IPLAYER_H
