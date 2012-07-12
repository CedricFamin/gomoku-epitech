#ifndef GOBAN_H
#define GOBAN_H

#include <QObject>
#include <QLabel>
#include <QPixmap>
#include <QRect>
#include <QMap>
#include <QString>
#include <QMouseEvent>
#include <QList>
#include <QMessageBox>
#include <QMainWindow>
#include <iostream>

#include "Referrer.h"
#include "iplayer.h"
#include "XYManager.h"
#include "PatternIdentifier.h"

struct Square
{
    bool isEmpty : 1;
    unsigned short x: 9;
    unsigned short y: 9;
    QRect rect;
    QLabel* image;
};


class GobanQt : public QLabel
{
    Q_OBJECT
public:
    explicit GobanQt(QMainWindow *parent, QPixmap Image);
    bool getPlayerTurn(void) const;
    int getInformation(void) const;
    void PlayAt(Goban::PION_TYPE color, unsigned int x, unsigned int y);
    void afterPlayer(void);
    IPlayer* currentPlayer(int turn);
    std::pair<int, int> move;
    void deleteStoneAt(unsigned int x, unsigned int y);
    unsigned int getX() const;
    unsigned int getY() const;
    unsigned short getCapturedStoneBlack() const;
    unsigned short getCapturedStoneWhite() const;
    void newGame();
	void showInfluence(Goban &g);
	void affPlayable(Goban &g, Referrer &r, Goban::PION_TYPE color);
	void affSelectedMove(Goban &g);
	template<int dir>
	void affEvalCase(Goban &g)		
	{
		Goban::PION_TYPE currentPion;
		Goban::Case toEval;
		int x = 0, y = 0;
		XYManager<dir> manager;
		manager.init(x, y);
		int nextEval = 0;

		while (!manager.ended(x, y))
		{
			toEval = g[y][x];
			int index = x + 19 * y;
			this->square[index].image->setAutoFillBackground(true);
			QPalette Pal(this->square[index].image->palette());
			Pal.setColor(QPalette::Window, QColor(20, 20, 20));
			this->square[index].image->setPalette(Pal);
			this->square[index].image->move(this->square[index].x, this->square[index].y);
			currentPion = (Goban::PION_TYPE)(toEval & Goban::PIONMASK);
			if (currentPion)
			{
				Patterns::PatternInfos pInfos1 = Patterns::patterns[(toEval >> Goban::HEADERSIZE >> Goban::PATTERNSIZE * dir) & Goban::PATTERNMASK];
				nextEval = pInfos1.size;
			}
			else
			{
				nextEval = 4;
			}
			manager.incremente(x, y, ++nextEval);
		}
	}
    ~GobanQt();
protected:
    void mousePressEvent (QMouseEvent* e);
private:
    bool playerTurn;
    Square square[361];
    int informations;
    Referrer * referrer;
    std::list<std::pair<unsigned int, unsigned int> > coordinates;
    IPlayer * _players[2];
    unsigned int x;
    unsigned int y;
    unsigned short capturedStoneBlack;
    unsigned short capturedStoneWhite;
signals:
    void clicked();
    void captured();
};

#endif // GOBAN_H
