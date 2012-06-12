#ifndef FINISHED_H
#define FINISHED_H

#include <QtGui/QDialog>

class Ui_Dialog;

class Finished: public QDialog
{
    Q_OBJECT

public:
    explicit Finished(QWidget *parent = 0);
    ~Finished();

private:
    Ui_Dialog *ui;

};

#endif // FINISHED_H
