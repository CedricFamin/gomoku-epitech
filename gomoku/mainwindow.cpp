#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Goban.h"
#include "Evaluator.h"

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
    connect(ui->actionDouble_three_rule, SIGNAL(triggered()), this, SLOT(double_Three_Rule()));
    connect(ui->actionEndgame_catch_rule, SIGNAL(triggered()), this, SLOT(endgame_Catch_Rule()));
    closed = false;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete label;
}

void MainWindow::new_Game()
{
    this->ui->label_8->setText(QString::number(0));
    this->ui->label_9->setText(QString::number(0));
    this->ui->label_19->setText(QString::number(0));
    this->ui->label_20->setText(QString::number(0));
    this->ui->textEdit->clear();
    this->label->newGame();
#ifdef _WIN32
    Sleep(100);
#else
    sleep(1);
#endif
}


void MainWindow::newGame()
{
    this->ui->label_8->setText(QString::number(0));
    this->ui->label_9->setText(QString::number(0));
    this->ui->label_19->setText(QString::number(0));
    this->ui->label_20->setText(QString::number(0));
    this->ui->textEdit->clear();
    this->label->newGame();
    emit this->newGameSignal();
}

void MainWindow::double_Three_Rule()
{
    if (this->ui->actionDouble_three_rule->isChecked())
        this->ui->actionDouble_three_rule->setChecked(true);
    else
        this->ui->actionDouble_three_rule->setChecked(false);
    emit this->doubleThreeRule();
}

void MainWindow::endgame_Catch_Rule()
{
    if (this->ui->actionEndgame_catch_rule->isChecked())
        this->ui->actionEndgame_catch_rule->setChecked(true);
    else
        this->ui->actionEndgame_catch_rule->setChecked(false);
    emit this->endgameCatchRule();
}

void MainWindow::checkTurn()
{
    ui->label_11->setText((this->playerTurn = this->label->getPlayerTurn()) ? "Player 2" : "Player 1");
}

void MainWindow::capturedStone()
{
    this->ui->label_20->setText(QString::number(this->label->getCapturedStoneBlack()));
    this->ui->label_19->setText(QString::number(this->label->getCapturedStoneWhite()));
    std::cout << this->ui->label_19->text().toStdString() << std::endl;
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

unsigned short MainWindow::getMode() const
{
    return this->mode;
}

void MainWindow::setWin(bool &affWon, History* history)
{
    this->affWon = affWon;
    this->ui->actionReplay_Game->setEnabled(true);
    this->history = history;
    connect(ui->actionReplay_Game, SIGNAL(triggered()), this, SLOT(replay_Game()));
}

void MainWindow::displayTextEdit(History::Turn& turn, Goban& goban)
{
    QString final;
    Evaluator eval(&goban);
    if (turn.pion == Goban::BLACK)
        final += QString("Black is ");
    else if (turn.pion == Goban::RED)
        final += QString("White is ");
    if (!turn.capture)
        final += QString("playing in ");
    else
        final += QString("captured in ");
    final += QString::number(turn.x);
    final += QString(", ");
    final += QString::number(turn.y);
    if (!turn.capture)
    {
        final += QString(" with an heuristic score of : ");
        final += QString::number(eval(goban, turn.pion));
    }
    this->ui->textEdit->append(final);
}

void MainWindow::replay_Game()
{
    this->ui->label_8->setText(QString::number(0));
    this->ui->label_9->setText(QString::number(0));
    this->ui->label_19->setText(QString::number(0));
    this->ui->label_20->setText(QString::number(0));
    this->ui->textEdit->clear();
    this->label->newGame();
    emit replayGame();
}

GobanQt & MainWindow::getGoban()
{
    return *this->label;
}
