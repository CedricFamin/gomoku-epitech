#include <functional>

#include "qtgoban.h"

#include "Rules/EachInTurnRule.h"
#include "Rules/DoubleThree.h"
#include "Rules/NotEmptyRule.h"
#include "Rules/TakingRules.h"
#include "Rules/VictoryCapturesRule.h"
#include "Rules/VictoryAlignment.h"
#include "AI/aiplayer.h"
#include "Finished.h"
#include "realplayer.h"

GobanQt::GobanQt(QMainWindow *parent, QPixmap Image) :
    QLabel(parent)
{
    this->setPixmap(Image);
    this->playerTurn = false;
    unsigned int x = 1;
    unsigned int y = 1;
    unsigned short id = 0;
    for (unsigned short i = 0; i < 19; ++i)
    {
        for (unsigned short j = 0; j < 19; ++j)
        {
            this->square[id].x = x;
            this->square[id].y = y;
            this->square[id].isEmpty = false;
            this->square[id].rect = QRect(x, y, 18, 18);
            this->square[id].image = new QLabel(this);
            this->square[id].image->setMinimumHeight(18);
            this->square[id].image->setMinimumWidth(18);
            ++id;
            x += 19;
        }
        x = 1;
        y += 19;
    }

    Goban *  goban = new Goban();
    this->referrer =  new Referrer(*goban);
    this->referrer->addPrePlayRule(*(new Rules::EachInTurnRule()));
    this->referrer->addPrePlayRule(*(new Rules::DoubleThree()));
    this->referrer->addPrePlayRule(*(new Rules::NotEmptyRule()));
    Rules::TakingRules * tkrule = new Rules::TakingRules();
    this->referrer->addPlayRule(*tkrule);
    this->referrer->addPostPlayRule(*(new Rules::VictoryCapturesRule(*tkrule)));
    this->referrer->addPostPlayRule(*(new Rules::VictoryAlignment()));
    this->_players[0] = new AIPlayer(Goban::BLACK);
    this->_players[1] = new RealPlayer(Goban::RED);
    this->currentPlayer()->play(*this->referrer, std::tr1::bind(&GobanQt::PlayAt, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2, std::tr1::placeholders::_3));

}

GobanQt::~GobanQt()
{
    for (unsigned short i = 0; i < 361; ++i)
        delete this->square[i].image;
}

void GobanQt::PlayAt(Goban::PION_TYPE color, unsigned int x, unsigned int y)
{
    QString pionImg;
    int index = x + y * 19;
    pionImg = (color == Goban::BLACK) ?":/new/prefix1/pionNoir.png" : ":/new/prefix1/pionBlanc.png";
    this->square[index].isEmpty = true;
    this->square[index].image->setPixmap(QPixmap(pionImg));
    this->square[index].image->move(this->square[index].x, this->square[index].y);
    playerTurn = !playerTurn;
    emit clicked();
    this->afterPlayer();
}

void GobanQt::mousePressEvent(QMouseEvent* e)
{

    if (e->button() == Qt::LeftButton)
    {
        for (unsigned short i = 0; i < 361; ++i)
        {
            if (this->square[i].y <= e->y() && this->square[i].x <= e->x() && (18 + this->square[i].x) >= e->x() && (this->square[i].y + 18) >= e->y())
            {
                if (this->referrer->CanPlay(this->currentPlayer()->getColor(), i % 19, i / 19))
                {
                    this->PlayAt(this->currentPlayer()->getColor(), i % 19, i/19);
                    this->referrer->Play();
                    this->informations = i;
                    this->referrer->AfterPlay();
                    this->currentPlayer()->play(*this->referrer, std::tr1::bind(&GobanQt::PlayAt, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2, std::tr1::placeholders::_3));
                    break;
                }

            }
        }

    }
}

void GobanQt::afterPlayer(void)
{
    Goban::PION_TYPE pion;

    if (this->referrer->GetListOfTurn().size())
    {
        this->coordinates = this->referrer->GetListOfTurn().back().captures;
        std::for_each(this->coordinates.begin(), this->coordinates.end(),
                [this](std::pair<unsigned int, unsigned int> & p)
        {
            unsigned int stoneToDelete = p.first + (p.second * 19);
            this->square[stoneToDelete].isEmpty = true;
            this->square[stoneToDelete].image->clear();
        });
    }

    if (this->referrer->GameFinished())
    {
        pion = this->referrer->Winner();
        Finished finish;
        finish.exec();
    }
}

int GobanQt::getInformation(void) const
{
    return this->informations;
}

bool GobanQt::getPlayerTurn(void) const
{
    return this->playerTurn;
}

IPlayer * GobanQt::currentPlayer()
{
    return this->_players[this->referrer->GetListOfTurn().size() & 1];
}
