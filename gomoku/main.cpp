#include  <QtGui/QApplication>
#ifdef _WIN32
    #include <Windows.h>
#endif
#include <QDebug>
#ifndef WIN32
#include <tr1/functional>
#else
#include <functional>
#endif

#include "connect_functor_helper.h"
#include "mainwindow.h"
#include "Goban.h"
#include "Rules/EachInTurnRule.h"
#include "Rules/DoubleThree.h"
#include "Rules/NotEmptyRule.h"
#include "Rules/TakingRules.h"
#include "Rules/VictoryCapturesRule.h"
#include "Rules/VictoryAlignment.h"
#include "AI/aiplayer.h"
#include "qtgoban.h"
#include "Finished.h"
#include "selectgametype.h"
#include "realplayer.h"
#include "history.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	Goban goban;
    SelectGameType gametype;
	MainWindow win;
    GobanQt & uiGoban = win.getGoban();
    History history;

    bool affWon = false;

	connect(&win, SIGNAL(newGameSignal()), std::tr1::bind(&Goban::clear, &goban));
	connect(&win, SIGNAL(newGameSignal()), [&affWon]{affWon = false;});

    Referrer referrer;
    Rules::TakingRules * tkrule = new Rules::TakingRules();
	Rules::DoubleThree* doubleThree = new Rules::DoubleThree;
    Rules::NotEmptyRule* notEmptyRule = new Rules::NotEmptyRule;
	Rules::VictoryAlignment * victoryAlignment = new Rules::VictoryAlignment;
    IPlayer * currentPlayer;
    IPlayer* players[2];
    referrer.addPrePlayRule(*(doubleThree));
    referrer.addPrePlayRule(*(notEmptyRule));
    referrer.addPlayRule(*tkrule);
	referrer.addPostPlayRule(*(new Rules::VictoryCapturesRule()));
    referrer.addPostPlayRule(*victoryAlignment);

    connect(&win, SIGNAL(doubleThreeRule()), [&doubleThree]
    {
            if (doubleThree->isEnable())
                doubleThree->disable();
            else
                doubleThree->enable();
    });
    connect(&win, SIGNAL(endgameCatchRule()), [&victoryAlignment]
    {
		static bool enable = true;
            if (enable)
				victoryAlignment->disableOptionalRule();
            else
				victoryAlignment->enableOptionalRule();
			enable = !enable;
    });
    /*connect(&win, SIGNAL(replayGame()), std::tr1::bind(&Goban::clear, &goban));
    connect(&win, SIGNAL(replayGame()), [&history, &uiGoban]
    {
        while (!history.turnList.empty())
        {
            if (!history.turnList.front().capture)
                uiGoban.deleteStoneAt(history.turnList.front().x, history.turnList.front().y);
            else
                uiGoban.PlayAt(history.turnList.front().pion, history.turnList.front().x, history.turnList.front().y, false);
            history.turnList.pop();
            #ifdef _WIN32
                Sleep(100);
            #else
                //sleep(1);
            #endif
        }
    });*/

    gametype.exec();
    win.show();
    if (gametype.getMode() == 0)
    {
        players[0] = new AIPlayer(Goban::BLACK);
        players[1] = new RealPlayer(Goban::RED, uiGoban);
    }
    else if (gametype.getMode() == 1)
    {
        players[0] = new AIPlayer(Goban::BLACK);
        players[1] = new AIPlayer(Goban::RED);
    }
    else if (gametype.getMode() == 2)
    {
        players[0] = new RealPlayer(Goban::BLACK, uiGoban);
        players[1] = new RealPlayer(Goban::RED, uiGoban);
    }
    else
    {
        players[0] = new AIPlayer(Goban::BLACK);
        players[1] = new RealPlayer(Goban::RED, uiGoban);
    }

    while (!win.IsClosed())
    {
        if (app.hasPendingEvents()) app.processEvents();
            currentPlayer = players[goban.nbTurn & 1];
        if (goban.gameFinished() == false)
        {
            Goban::Move move = currentPlayer->play(referrer, goban);
			//Comment j'insere ce code en dessous? :D
			// comme ça x)
			if (goban.InBound(move.first, move.second))
			{
				uiGoban.PlayAt(currentPlayer->getColor(), move.first, move.second);
				History::Turn tmpTurn;
				tmpTurn.pion = currentPlayer->getColor();
				tmpTurn.x = uiGoban.getX();
				tmpTurn.y = uiGoban.getY();
				tmpTurn.capture = false;
                win.displayTextEdit(tmpTurn, goban);
				history.turnList.push(tmpTurn);
			}

        }
        while (!tkrule->toDelete.empty())
        {
            History::Turn tmpTurn;
			std::pair<unsigned int, unsigned int>  p = tkrule->toDelete.top();
			tkrule->toDelete.pop();
			uiGoban.deleteStoneAt(p.first, p.second);
			tmpTurn.pion = Goban::Other(currentPlayer->getColor());
            tmpTurn.x = p.first;
            tmpTurn.y = p.second;
            tmpTurn.capture = true;
            win.displayTextEdit(tmpTurn, goban);
            history.turnList.push(tmpTurn);
        }
        if (goban.gameFinished() && !affWon)
        {
            Finished finish;
            finish.exec();
            affWon = true;
            win.setWin(affWon, &history);
        }
//#if _DEBUG
		
        //uiGoban.affPlayable(goban, referrer, currentPlayer->getColor());
        //uiGoban.showInfluence(goban);
		//uiGoban.affSelectedMove(goban);
		//uiGoban.affEvalCase<0>(goban);
		//uiGoban.affEvalCase<1>(goban);
		//uiGoban.affEvalCase<2>(goban);
		//uiGoban.affEvalCase<3>(goban);
//#endif
#ifdef _WIN32
        //Sleep(100);
#else
        //sleep(1);
#endif
    }
    app.exit();
    return 0;
}
