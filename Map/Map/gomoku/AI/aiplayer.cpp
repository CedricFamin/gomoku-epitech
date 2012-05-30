#include <algorithm>
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
    std::list<std::pair<unsigned int, unsigned int> > turns = _getTurns(r.getGoban(), r, this->getColor());
    bool played = false;

    std::for_each(turns.begin(), turns.end(),
    [this, &played, &r, &callback](std::pair<unsigned int, unsigned int> & coord)
    {
        if (played)
              return ;
        if (r.CanPlay(this->_color,coord.first, coord.second))
        {
            played = true;
            r.Play();
            r.AfterPlay();
            callback(this->_color, coord.first, coord.second);
        }
    });


}

std::list<std::pair<unsigned int, unsigned int> > AIPlayer::_getTurns(Goban & g, Referrer & r ,Goban::PION_TYPE pion)
{
    const int direction[8][2] = {
            { 0,-1}, { 1, -1}, { 1, 0}, { 1, 1},
            {0, 1}, {-1, 1}, { -1,0}, { -1,-1}
        };

    std::list<std::pair<unsigned int, unsigned int> > possiblesTurns;

    if (r.GetListOfTurn().size() == 0)
    {
        possiblesTurns.push_front(std::make_pair(9,9));
        return possiblesTurns;
    }
    Referrer::Turn lastTurn = r.GetListOfTurn().back();

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 1; j < 5; ++j)
        {
            if (g.InBound(lastTurn.x + direction[i][0] * j, lastTurn.y + direction[i][1] * j))
                possiblesTurns.push_front(std::make_pair(lastTurn.x + direction[i][0] * j,
                                                         lastTurn.y + direction[i][1] * j));
        }
    }

    return possiblesTurns;
}
