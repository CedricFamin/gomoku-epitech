#include  <QtGui/QApplication>
#include <Windows.h>
#include <QDebug>

#include "connect_functor_helper.h"
#include "mainwindow.h"
#include "goban.h"
#include "Rules/EachInTurnRule.h"
#include "Rules/DoubleThree.h"
#include "Rules/NotEmptyRule.h"
#include "Rules/TakingRules.h"
#include "Rules/VictoryCapturesRule.h"
#include "Rules/VictoryAlignment.h"
#include "AI/aiplayer.h"
#include "qtgoban.h"
#include "Finished.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow win;
    GobanQt & uiGoban = win.getGoban();

    Goban goban;
    Referrer referrer(goban);
    Rules::TakingRules * tkrule = new Rules::TakingRules();
    IPlayer * currentPlayer;
    bool closed = false;

    referrer.addPrePlayRule(*(new Rules::EachInTurnRule()));
    //referrer.addPrePlayRule(*(new Rules::DoubleThree()));
    referrer.addPrePlayRule(*(new Rules::NotEmptyRule()));
    referrer.addPlayRule(*tkrule);
    referrer.addPostPlayRule(*(new Rules::VictoryCapturesRule(*tkrule)));
    referrer.addPostPlayRule(*(new Rules::VictoryAlignment()));

    win.show();
    while (!win.IsClosed())
    {
        currentPlayer = uiGoban.currentPlayer(referrer.GetListOfTurn().size());
        if (app.hasPendingEvents())
            app.processEvents();
        currentPlayer->play(referrer, std::tr1::bind(&GobanQt::PlayAt, &uiGoban, std::tr1::placeholders::_1, std::tr1::placeholders::_2, std::tr1::placeholders::_3));

        if (referrer.GetListOfTurn().size())
        {
            auto coordinates = referrer.GetListOfTurn().back().captures;
            std::for_each(coordinates.begin(), coordinates.end(),
                    [&uiGoban](std::pair<unsigned int, unsigned int> & p)
            {
                          uiGoban.deleteStoneAt(p.first, p.second);
            });
        }
        if (referrer.GameFinished())
        {
            Finished finish;
            finish.exec();
        }
        Sleep(100);
    }
    app.exit();
    return 0;
}
