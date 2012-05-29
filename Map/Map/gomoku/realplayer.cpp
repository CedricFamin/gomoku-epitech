#include "realplayer.h"

RealPlayer::RealPlayer(Goban::PION_TYPE pion) : _color(pion)
{
}

Goban::PION_TYPE RealPlayer::getColor() const
{
    return this->_color;
}

void RealPlayer::play(Referrer &, callback_type callback)
{

}

