#include "realplayer.h"

RealPlayer::RealPlayer(Goban::PION_TYPE pion, GobanQt & ui) : _color(pion), _ui(ui)
{
}

Goban::PION_TYPE RealPlayer::getColor() const
{
    return this->_color;
}

void RealPlayer::play(Referrer & r, Goban & g, callback_type callback)
{
	if (this->_ui.move.first == -1 || this->_ui.move.second == -1)
		return ;
    if (r(g, this->getColor(), this->_ui.move.first, this->_ui.move.second))
    {
        callback(this->getColor(), this->_ui.move.first, this->_ui.move.second);
		this->_ui.move.first = -1;
		this->_ui.move.second = -1;
    }
}

