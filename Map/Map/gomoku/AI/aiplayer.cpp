#include "aiplayer.h"

AIPlayer::AIPlayer(Goban::PION_TYPE pion) : _color(pion)
{

}

Goban::PION_TYPE AIPlayer::getColor() const
{
    return this->_color;
}
void AIPlayer::play(Referrer & r, callback_type callback)
{
    unsigned int x, y;
    do
    {
        x = rand() % 19;
        y = rand() % 19;
    } while (r.CanPlay(this->_color, x, y) == false);
    r.Play();
    r.AfterPlay();
    callback(this->_color, x, y);

}
