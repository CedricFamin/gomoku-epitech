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
    
private:
    Ui::MainWindow *ui;
    GobanQt *label;
    bool playerTurn;

private slots:
    void checkTurn();
    void newGame();
    void displaySquareInformations();
};

#endif // MAINWINDOW_H
