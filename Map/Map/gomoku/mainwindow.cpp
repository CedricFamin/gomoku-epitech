#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "goban.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1063, 648);
    this->setStyleSheet("QMainWindow {image : url(:/new/prefix1/creation.png)}");
    label = new GobanQt(this, QString(":/new/prefix1/goban.gif"));
    ui->verticalLayout_2->insertWidget(0, label);
    label->show();
    connect(this->label, SIGNAL(clicked()), this, SLOT(checkTurn()));
    connect(ui->actionNew_game, SIGNAL(triggered()), this, SLOT(newGame()));
    connect(this->label, SIGNAL(clicked()), this, SLOT(displaySquareInformations()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete label;
}

void MainWindow::checkTurn()
{
    this->playerTurn = this->label->getPlayerTurn();
    if (!this->playerTurn)
        ui->label_11->setText(QString("Player 1"));
    else
        ui->label_11->setText(QString("Player 2"));
}

void MainWindow::displaySquareInformations()
{
    int informationSquare = this->label->getInformation();
    this->ui->label_6->text();
    this->ui->label_7->text();
    this->ui->label_8->setText(QString::number(informationSquare / 19));
    this->ui->label_9->setText(QString::number(informationSquare % 19));
}

void MainWindow::newGame()
{

}
