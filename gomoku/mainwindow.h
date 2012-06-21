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

signals:
	void newGameSignal();

protected:
    void closeEvent(QCloseEvent* event);
    
private:
    Ui::MainWindow *ui;
    GobanQt *label;
    bool playerTurn;
    bool closed;

private slots:
    void checkTurn();
    void displaySquareInformations();
    void capturedStone();
    void newGame();
};

#endif // MAINWINDOW_H
