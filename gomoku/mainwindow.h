#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QPixmap>

#include "qtgoban.h"
#include "history.h"

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
    void setWin(bool&, History *history);
    void displayTextEdit(History::Turn &turn, Goban& goban);
    void new_Game();

signals:
	void newGameSignal();
    void doubleThreeRule();
    void endgameCatchRule();
    void replayGame();

protected:
    void closeEvent(QCloseEvent* event);
    
private:
    Ui::MainWindow *ui;
    GobanQt *label;
    bool playerTurn;
    bool closed;
    unsigned short mode:2;
    bool affWon;
    History* history;

private slots:
    void checkTurn();
    void displaySquareInformations();
    void replay_Game();

    void capturedStone();
    void newGame();
    void double_Three_Rule();
    void endgame_Catch_Rule();
};

#endif // MAINWINDOW_H
