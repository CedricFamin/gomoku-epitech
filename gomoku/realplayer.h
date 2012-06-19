#ifndef REALPLAYER_H
#define REALPLAYER_H

#include "iplayer.h"
#include "qtgoban.h"

class RealPlayer : public IPlayer
{
public:
    RealPlayer(Goban::PION_TYPE, GobanQt &ui);
    virtual Goban::PION_TYPE getColor() const;
    virtual void play(Referrer &, Goban & g, callback_type);

protected:
private:
    Goban::PION_TYPE _color;
    GobanQt & _ui;
};

#endif // REALPLAYER_H
