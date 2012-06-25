#include "Finished.h"
#include "ui_Finished.h"
#include <QPixmap>
#include <QIcon>

Finished::Finished(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_Finished)
{
    ui->setupUi(this);
    this->pixmap = new QPixmap(":/new/prefix1/win.jpg");
    QIcon icon(*pixmap);
    QSize iconSize(pixmap->width(), pixmap->height());
    ui->pushButton->setIconSize(iconSize);
    ui->pushButton->setIcon(icon);
}

Finished::~Finished()
{
    delete pixmap;
    delete ui;
}
