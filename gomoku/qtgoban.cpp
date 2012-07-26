#include <algorithm>
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
#include "AI/alphabetathreading.h"

GobanQt::GobanQt(QMainWindow *parent, QPixmap Image) :
    QLabel(parent)
{
    this->setPixmap(Image);
    this->playerTurn = false;
    unsigned int x = 1;
    unsigned int y = 1;
    unsigned short id = 0;
    this->capturedStoneBlack = 0;
    this->capturedStoneWhite = 0;
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
    this->_players[0] = new AIPlayer(Goban::BLACK);
    this->_players[1] = new AIPlayer(Goban::RED);
    //this->_players[0] = new RealPlayer(Goban::BLACK, *this);
    //this->_players[1] = new RealPlayer(Goban::RED, *this);
    move.first = -1;
    move.second = -1;
}

unsigned int GobanQt::getX() const
{
    return this->x;
}

unsigned int GobanQt::getY() const
{
    return this->y;
}

GobanQt::~GobanQt()
{
    for (unsigned short i = 0; i < 361; ++i)
        delete this->square[i].image;
}

void GobanQt::PlayAt(Goban::PION_TYPE color, unsigned int x, unsigned int y, bool history)
{
    QString pionImg;
    this->x = x;
    this->y = y;
    int index = x + y * 19;
    pionImg = (color == Goban::BLACK) ?":/new/prefix1/pionNoir.png" : ":/new/prefix1/pionBlanc.png";
    this->square[index].isEmpty = true;
    this->square[index].image->setPixmap(QPixmap(pionImg));
    this->square[index].image->move(this->square[index].x, this->square[index].y);
    playerTurn = !playerTurn;
    if (history)
        emit clicked();
    //this->afterPlayer();
}

void GobanQt::showInfluence(Goban &g)
{
	int index;
	for (unsigned int y = 0; y < g.getHeight(); ++y)
	{
		for (unsigned int x = 0; x < g.getWidth(); ++x)
		{
			index = x + 19 * y;
			this->square[x + 19 * y].image->setAutoFillBackground(true);

			QPalette Pal(this->square[index].image->palette());
			Goban::Case blackInfluence = ((g[y][x]>>Goban::BLACKINFLUENCEINDEX) & Goban::INFLUENCEMASK);
			Goban::Case whiteInfluence = ((g[y][x]>>Goban::WHITEINFLUENCEINDEX) & Goban::INFLUENCEMASK);
			Pal.setColor(QPalette::Window, QColor(255 - std::min<int>(blackInfluence * 0x1A, 255), 
				255 - std::min<int>(whiteInfluence * 0x1A, 255),
				255 - std::min<int>(blackInfluence * 0x1A, 255), 255));
			this->square[index].image->setPalette(Pal);
			this->square[index].image->move(this->square[index].x, this->square[index].y);
		}
	}
}


void GobanQt::affSelectedMove(Goban &g)
{
	auto moves = AlphaBetaThreading::GetTurns(g);
	std::for_each(moves.begin(), moves.end(),
	[this, &g](Goban::Move & m)
	{
		int index = m.first + 19 * m.second;
		this->square[index].image->setAutoFillBackground(true);
		QPalette Pal(this->square[index].image->palette());
		Pal.setColor(QPalette::Window, QColor(20, 20, 20, 25));
		this->square[index].image->setPalette(Pal);
		this->square[index].image->move(this->square[index].x, this->square[index].y);
	});
}

void GobanQt::affPlayable(Goban &g, Referrer &r, Goban::PION_TYPE color)
{
	int index;
	for (unsigned int y = 0; y < g.getHeight(); ++y)
	{
		for (unsigned int x = 0; x < g.getWidth(); ++x)
		{
			Goban s = g;
			index = x + 19 * y;
			this->square[x + 19 * y].image->setAutoFillBackground(true);
			QPalette Pal(this->square[index].image->palette());
			if (r(s, color, x, y) == false) Pal.setColor(QPalette::Window, QColor(255, 0, 0, 25));
			else Pal.setColor(QPalette::Window, QColor(255, 255, 255, 25));
			this->square[index].image->setPalette(Pal);
			this->square[index].image->move(this->square[index].x, this->square[index].y);
		}
	}
}

void GobanQt::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        for (unsigned short i = 0; i < 361; ++i)
        {
            if (this->square[i].y <= e->y() && this->square[i].x <= e->x() && (18 + this->square[i].x) >= e->x() && (this->square[i].y + 18) >= e->y())
            {
                move.first = i % 19;
                move.second = i / 19;
                /*if (this->referrer->CanPlay(this->currentPlayer()->getColor(), i % 19, i / 19))
                {
                    Goban::PION_TYPE pion = this->currentPlayer()->getColor();
                    this->referrer->Play();
                    this->informations = i;
                    this->referrer->AfterPlay();
                    this->PlayAt(pion, i % 19, i/19);
                    break;
                }*/
            }
        }

    }
}

void GobanQt::deleteStoneAt(unsigned int x, unsigned int y)
{
    playerTurn ? this->capturedStoneBlack += 1 : this->capturedStoneWhite += 1;
    unsigned int stoneToDelete = x + (y * 19);
    this->square[stoneToDelete].isEmpty = true;
    this->square[stoneToDelete].image->clear();
    emit captured();
}

void GobanQt::afterPlayer(void)
{
    //Goban::PION_TYPE pion;

    /*if (this->referrer->GetListOfTurn().size())
    {
        this->coordinates = this->referrer->GetListOfTurn().back().captures;
        std::for_each(this->coordinates.begin(), this->coordinates.end(),
                [this](std::pair<unsigned int, unsigned int> & p)
        {
            unsigned int stoneToDelete = p.first + (p.second * 19);
            this->square[stoneToDelete].isEmpty = true;
            this->square[stoneToDelete].image->clear();
        });
    }*/
}

int GobanQt::getInformation(void) const
{
    return this->informations;
}

bool GobanQt::getPlayerTurn(void) const
{
    return this->playerTurn;
}

unsigned short GobanQt::getCapturedStoneBlack() const
{
    return this->capturedStoneBlack;
}

unsigned short GobanQt::getCapturedStoneWhite() const
{
    return this->capturedStoneWhite;
}

void GobanQt::newGame()
{
    this->playerTurn = false;
    this->x = 0;
    this->y = 0;
    this->capturedStoneBlack = 0;
    this->capturedStoneWhite = 0;
    for (unsigned short i = 0; i < 361; ++i)
    {
        this->square[i].isEmpty = true;
        this->square[i].image->clear();
    }
}

IPlayer * GobanQt::currentPlayer(int turn)
{
    return this->_players[turn & 1];
}
