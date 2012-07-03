#include "selectgametype.h"
#include "ui_selectgametype.h"
#include "mainwindow.h"

SelectGameType::SelectGameType(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectGameType)
{
    ui->setupUi(this);
    this->PvIpixmap = new QPixmap(":/new/prefix1/PlayerVsIa.jpg");
    this->IvIpixmap = new QPixmap(":/new/prefix1/IaVsIa.jpg");
    this->PvPpixmap = new QPixmap(":/new/prefix1/PlayerVsPlayer.png");
    QIcon PvIicon(*PvIpixmap);
    QIcon IvIicon(*IvIpixmap);
    QIcon PvPicon(*PvPpixmap);
    QSize PvIiconSize(399, 266);
    QSize IvIiconSize(399, 266);
    QSize PvPiconSize(PvPpixmap->width(), PvPpixmap->height());
    ui->PlayerVSIA->setIconSize(PvIiconSize);
    ui->IAVSIA->setIconSize(IvIiconSize);
    ui->PlayerVSPlayer->setIconSize(PvPiconSize);
    ui->PlayerVSIA->setIcon(PvIicon);
    ui->IAVSIA->setIcon(IvIicon);
    ui->PlayerVSPlayer->setIcon(PvPicon);
    connect(ui->PlayerVSIA, SIGNAL(clicked()), this, SLOT(PlayerVSIa()));
    connect(ui->IAVSIA, SIGNAL(clicked()), this, SLOT(IaVSIa()));
    connect(ui->PlayerVSPlayer, SIGNAL(clicked()), this, SLOT(PlayerVSPlayer()));
    connect(ui->PlayerVSIA, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->IAVSIA, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->PlayerVSPlayer, SIGNAL(clicked()), this, SLOT(close()));
}

SelectGameType::~SelectGameType()
{
    delete ui;
    delete this->PvIpixmap;
    delete this->IvIpixmap;
    delete this->PvPpixmap;
}

void SelectGameType::PlayerVSIa()
{
    this->mode = 0;
}

void SelectGameType::IaVSIa()
{
    this->mode = 1;
}

void SelectGameType::PlayerVSPlayer()
{
    this->mode = 2;
}

int SelectGameType::getMode() const
{
    return this->mode;
}
