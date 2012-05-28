#ifndef REALPLAYER_H
#define REALPLAYER_H

#include "iplayer.h"

class RealPlayer : public IPlayer
{
public:
    RealPlayer(Goban::PION_TYPE);
    virtual Goban::PION_TYPE getColor() const;
    virtual void play(Referrer & r);

protected:
private:
    Goban::PION_TYPE _color;
};

#endif // REALPLAYER_H
