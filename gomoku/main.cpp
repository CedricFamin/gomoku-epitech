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

/*class Game : public QThread
{
public:
    Game(GobanQt & gobanUI) : _gobanUI(gobanUI), _referrer(_goban), _victoryCapture(_takingRule)
    {
        _referrer.addPrePlayRule(_notEmpty);
        _referrer.addPrePlayRule(_eitr);
        _referrer.addPrePlayRule(_double3);
        _referrer.addPlayRule(_takingRule);
        _referrer.addPostPlayRule(_victoryCapture);
        _referrer.addPostPlayRule(_victoryAlignment);
    }

    void run()
    {
        IPlayer * currentPlayer;
        bool affWon = false;

        while (1)
        {
            currentPlayer = this->_gobanUI.currentPlayer(this->_referrer.GetListOfTurn().size());
            if (this->_referrer.GameFinished() == false)
                currentPlayer->play(this->_referrer, std::tr1::bind(&GobanQt::PlayAt, &this->_gobanUI, std::tr1::placeholders::_1, std::tr1::placeholders::_2, std::tr1::placeholders::_3));

            if (this->_referrer.GetListOfTurn().size())
            {
                auto coordinates = this->_referrer.GetListOfTurn().back().captures;
                std::for_each(coordinates.begin(), coordinates.end(),
                [this](std::pair<unsigned int, unsigned int> & p)
                {
                    _gobanUI.deleteStoneAt(p.first, p.second);
                });
            }
            if (this->_referrer.GameFinished() && !affWon)
            {
                Finished finish;
                finish.exec();
                affWon = true;
            }
            Sleep(100);
        }
    }

private:
    GobanQt & _gobanUI;
    Goban _goban;
    Rules::EachInTurnRule _eitr;
    Rules::DoubleThree _double3;
    Rules::NotEmptyRule _notEmpty;
    Rules::TakingRules _takingRule;
    Rules::VictoryCapturesRule _victoryCapture;
    Rules::VictoryAlignment _victoryAlignment;
    Referrer _referrer;

};*/


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	Goban goban;
	MainWindow win;
    GobanQt & uiGoban = win.getGoban();
    bool affWon = false;

	connect(&win, SIGNAL(newGameSignal()), std::tr1::bind(&Goban::clear, &goban));
	connect(&win, SIGNAL(newGameSignal()), [&affWon]{affWon = false;});

    Referrer referrer;
    Rules::TakingRules * tkrule = new Rules::TakingRules();
    IPlayer * currentPlayer;
    referrer.addPrePlayRule(*(new Rules::EachInTurnRule()));
    referrer.addPrePlayRule(*(new Rules::DoubleThree()));
    referrer.addPrePlayRule(*(new Rules::NotEmptyRule()));
    referrer.addPlayRule(*tkrule);
    referrer.addPostPlayRule(*(new Rules::VictoryCapturesRule()));
    referrer.addPostPlayRule(*(new Rules::VictoryAlignment()));
	
    win.show();
    while (!win.IsClosed())
    {
		if (app.hasPendingEvents()) app.processEvents();
		currentPlayer = uiGoban.currentPlayer(goban.Turns().size());
		if (goban.gameFinished() == false)
            currentPlayer->play(referrer, goban, std::tr1::bind(&GobanQt::PlayAt, &uiGoban, std::tr1::placeholders::_1, std::tr1::placeholders::_2, std::tr1::placeholders::_3));

        if (goban.Turns().size())
        {
            auto coordinates = goban.Turns().back().captures;
            std::for_each(coordinates.begin(), coordinates.end(),
            [&uiGoban](std::pair<unsigned int, unsigned int> & p)
            {
                uiGoban.deleteStoneAt(p.first, p.second);
            });
        }
        if (goban.gameFinished() && !affWon)
        {
            Finished finish;
            finish.exec();
            affWon = true;
        }
//#if _DEBUG
		uiGoban.showInfluence(goban);
//#endif
#ifdef _WIN32
        Sleep(100);
#else
        //sleep(1);
#endif
    }
    app.exit();
    return 0;
}
