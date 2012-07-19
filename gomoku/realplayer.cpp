#include <QDebug>
#include "realplayer.h"
#include "Evaluator.h"

RealPlayer::RealPlayer(Goban::PION_TYPE pion, GobanQt & ui) : _color(pion), _ui(ui)
{
}

Goban::PION_TYPE RealPlayer::getColor() const
{
    return this->_color;
}

Goban::Move RealPlayer::play(Referrer & r, Goban & g)
{
	Evaluator eval(&g);
	if (this->_ui.move.first == -1 || this->_ui.move.second == -1)
		return Goban::Move(-1, -1);
    if (r(g, this->getColor(), this->_ui.move.first, this->_ui.move.second))
    {
		qDebug() << this->_color << ":" << eval(g, this->_color);
		return this->_ui.move;

    }
	return Goban::Move(-1, -1);
}

