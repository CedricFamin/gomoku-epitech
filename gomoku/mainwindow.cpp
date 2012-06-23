#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Goban.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1063, 648);
    ui->label_19->setText(QString::number(0));
    ui->label_20->setText(QString::number(0));
    this->setStyleSheet("QMainWindow {image : url(:/new/prefix1/creation.png)}");
    label = new GobanQt(this, QString(":/new/prefix1/goban.gif"));
    ui->verticalLayout_2->insertWidget(0, label);
    label->show();
    connect(this->label, SIGNAL(clicked()), this, SLOT(checkTurn()));
    connect(ui->actionNew_game, SIGNAL(triggered()), this, SLOT(newGame()));
    connect(this->label, SIGNAL(clicked()), this, SLOT(displaySquareInformations()));
    connect(this->label, SIGNAL(captured()), this, SLOT(capturedStone()));
    closed = false;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete label;
}

void MainWindow::newGame()
{
    this->ui->label_8->setText(QString::number(0));
    this->ui->label_9->setText(QString::number(0));
    this->ui->label_19->setText(QString::number(0));
    this->ui->label_20->setText(QString::number(0));
    this->label->newGame();
	emit this->newGameSignal();
}

void MainWindow::checkTurn()
{
    ui->label_11->setText((this->playerTurn = this->label->getPlayerTurn()) ? "Player 2" : "Player 1");
}

void MainWindow::capturedStone()
{
    this->ui->label_20->setText(QString::number(this->label->getCapturedStoneBlack()));
    this->ui->label_19->setText(QString::number(this->label->getCapturedStoneWhite()));
}

void MainWindow::displaySquareInformations()
{
    unsigned int x = this->label->getX();
    unsigned int y = this->label->getY();
    this->ui->label_6->text();
    this->ui->label_7->text();
    this->ui->label_8->setText(QString::number(x));
    this->ui->label_9->setText(QString::number(y));
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    event->accept();
    closed = true;
}

bool MainWindow::IsClosed() const
{
    return this->closed;
}

GobanQt & MainWindow::getGoban()
{
    return *this->label;
}
