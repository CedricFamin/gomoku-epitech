#ifndef SELECTGAMETYPE_H
#define SELECTGAMETYPE_H

#include <QDialog>
#include <QPixmap>

namespace Ui {
class SelectGameType;
}

class SelectGameType : public QDialog
{
    Q_OBJECT
    
public:
    explicit SelectGameType(QWidget *parent = 0);
    ~SelectGameType();
    int getMode() const;
public slots:
    void PlayerVSIa();
    void IaVSIa();
    void PlayerVSPlayer();
    
private:
    Ui::SelectGameType *ui;
    QPixmap* PvIpixmap;
    QPixmap* IvIpixmap;
    QPixmap* PvPpixmap;
    int mode;
};

#endif // SELECTGAMETYPE_H
