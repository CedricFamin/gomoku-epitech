#include  <QtGui/QApplication>
#include <Windows.h>

#include "mainwindow.h"
#include "goban.h"
#include "Rules/EachInTurnRule.h"
#include "Rules/DoubleThree.h"
#include "Rules/NotEmptyRule.h"
#include "Rules/TakingRules.h"
#include "Rules/VictoryCapturesRule.h"
#include "Rules/VictoryAlignment.h"
#include "AI/aiplayer.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow win;
    win.show();
    Goban goban;
    Referrer referrer(goban);
    Rules::TakingRules * tkrule = new Rules::TakingRules();

    referrer.addPrePlayRule(*(new Rules::EachInTurnRule()));
    //this->referrer->addPrePlayRule(*(new Rules::DoubleThree()));
    referrer.addPrePlayRule(*(new Rules::NotEmptyRule()));
    referrer.addPlayRule(*tkrule);
    referrer.addPostPlayRule(*(new Rules::VictoryCapturesRule(*tkrule)));
    referrer.addPostPlayRule(*(new Rules::VictoryAlignment()));

    while (true)
    {
        app.processEvents();

        Sleep(100);
    }

    return 0;
}
