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


#include <sstream>
void generate_pattern()
{
	char pattern[3] = { '_', 'o', 'x' };
	int combin[4] = { 0, 0, 0, 0};

	for (int i = 0; i <81; ++i)
	{
		std::stringstream str;
		str << pattern[combin[0]] << pattern[combin[1]] << pattern[combin[2]] << pattern[combin[3]];
		qDebug() << str.str().c_str();
		if (++combin[0] > 2)
		{
			combin[0] = 0;
			if (++combin[1] > 2)
			{
				combin[1] = 0;
				if (++combin[2] > 2)
				{
					combin[2] = 0;
					++combin[3];
				}
			}

		}
	}
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	Goban goban;
    SelectGameType gametype;
	MainWindow win;
    GobanQt & uiGoban = win.getGoban();
    History history;

    bool affWon = false;

    Rules::DoubleThree* doubleThree = new Rules::DoubleThree;
    Rules::NotEmptyRule* notEmptyRule = new Rules::NotEmptyRule;
	connect(&win, SIGNAL(newGameSignal()), std::tr1::bind(&Goban::clear, &goban));
	connect(&win, SIGNAL(newGameSignal()), [&affWon]{affWon = false;});

    Referrer referrer;
    Rules::TakingRules * tkrule = new Rules::TakingRules();
    IPlayer * currentPlayer;
    IPlayer* players[2];
    //referrer.addPrePlayRule(*(new Rules::EachInTurnRule()));
    referrer.addPrePlayRule(*(doubleThree));
    referrer.addPrePlayRule(*(notEmptyRule));
    referrer.addPlayRule(*tkrule);
    referrer.addPostPlayRule(*(new Rules::VictoryCapturesRule()));
    referrer.addPostPlayRule(*(new Rules::VictoryAlignment()));

    connect(&win, SIGNAL(doubleThreeRule()), [&doubleThree]
    {
            if (doubleThree->isEnable())
                doubleThree->disable();
            else
                doubleThree->enable();
    });
    connect(&win, SIGNAL(endgameCatchRule()), [&notEmptyRule]
    {
            if (notEmptyRule->isEnable())
                notEmptyRule->disable();
            else
                notEmptyRule->enable();
    });

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
            currentPlayer = players[goban.Turns().size() & 1];
        if (goban.gameFinished() == false)
        {
            currentPlayer->play(referrer, goban, std::tr1::bind(&GobanQt::PlayAt, &uiGoban, std::tr1::placeholders::_1, std::tr1::placeholders::_2, std::tr1::placeholders::_3));
//Comment j'insere ce code en dessous? :D



            /*            History::Turn tmpTurn;
            tmpTurn.pion = currentPlayer->getColor();
            tmpTurn.x = uiGoban.getX();
            tmpTurn.y = uiGoban.getY();
            tmpTurn.capture = false;
            win.displayTextEdit(tmpTurn);
            history.turnList.push(tmpTurn);*/
        }
        while (!goban.toDelete.empty())
        {
            History::Turn tmpTurn;
			std::pair<unsigned int, unsigned int>  p = goban.toDelete.top();
			goban.toDelete.pop();
			uiGoban.deleteStoneAt(p.first, p.second);
            if (currentPlayer->getColor() == Goban::BLACK)
                tmpTurn.pion = Goban::RED;
            else
                tmpTurn.pion = Goban::BLACK;
            tmpTurn.x = p.first;
            tmpTurn.y = p.second;
            tmpTurn.capture = true;
            win.displayTextEdit(tmpTurn);
            history.turnList.push(tmpTurn);
        }
        if (goban.gameFinished() && !affWon)
        {
            Finished finish;
            finish.exec();
            affWon = true;
            win.setWin(affWon, history);
        }
//#if _DEBUG
		
		uiGoban.affPlayable(goban, referrer, currentPlayer->getColor());
		//uiGoban.showInfluence(goban);
		//uiGoban.affSelectedMove(goban);
		//uiGoban.affEvalCase<0>(goban);
		//uiGoban.affEvalCase<1>(goban);
		//uiGoban.affEvalCase<2>(goban);
		//uiGoban.affEvalCase<3>(goban);
//#endif
#ifdef _WIN32
        Sleep(100);
#else
        //sleep(1);
#endif
    }
    delete doubleThree;
    delete notEmptyRule;
    app.exit();
    return 0;
}
