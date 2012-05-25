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
    ~GobanQt();
protected:
    void mousePressEvent (QMouseEvent* e);
private:
    bool playerTurn;
    Square square[361];
    int informations;
    Referrer * referrer;
    std::list<std::pair<unsigned int, unsigned int> > coordinates;
signals:
    void clicked();
};

#endif // GOBAN_H
