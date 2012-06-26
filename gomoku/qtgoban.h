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
