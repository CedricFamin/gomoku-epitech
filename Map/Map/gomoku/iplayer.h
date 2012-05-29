#ifndef IPLAYER_H
#define IPLAYER_H

#include "goban.h"
#include "Referrer.h"

class IPlayer
{
public:
    typedef std::function<void(Goban::PION_TYPE, int, int)> callback_type;
    virtual Goban::PION_TYPE getColor() const = 0;
    virtual void play(Referrer & r, callback_type) = 0;
};

#endif // IPLAYER_H
