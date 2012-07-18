#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QPixmap>

#include "qtgoban.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool IsClosed() const;
    GobanQt & getGoban();
    unsigned short getMode() const;
    void setWin(bool&);

signals:
	void newGameSignal();
    void doubleThreeRule();
    void endgameCatchRule();

protected:
    void closeEvent(QCloseEvent* event);
    
private:
    Ui::MainWindow *ui;
    GobanQt *label;
    bool playerTurn;
    bool closed;
    unsigned short mode:2;
    bool affWon;

private slots:
    void checkTurn();
    void displaySquareInformations();
    void capturedStone();
    void newGame();
    void double_Three_Rule();
    void endgame_Catch_Rule();
};

#endif // MAINWINDOW_H
