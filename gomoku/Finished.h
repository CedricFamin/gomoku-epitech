#ifndef FINISHED_H
#define FINISHED_H

#include <QtGui/QDialog>

class Ui_Finished;

class Finished: public QDialog
{
    Q_OBJECT

public:
    explicit Finished(QWidget *parent = 0);
    ~Finished();

private:
    Ui_Finished *ui;

};

#endif // FINISHED_H
