#ifndef _REFERRER_H_
# define _REFERRER_H_

#include <list>
#include <utility>
#include "Goban.h"
#include "IRule.h"

class Referrer
{
public:
	struct Turn
	{
		unsigned int x;
		unsigned int y;
		Goban::PION_TYPE pion;
		bool validate;
        std::list<std::pair<unsigned int, unsigned int> > captures;
	};

	Referrer(Goban &);
	~Referrer(void);

	bool CanPlay(Goban::PION_TYPE, int, int);
	bool Play();
	bool AfterPlay();
	bool GameFinished() const;
	void setGameFinished(bool);
	Goban::PION_TYPE Winner() const;
	void setWinner(Goban::PION_TYPE);
	
	void addPrePlayRule(Rules::IRule &);
	void addPlayRule(Rules::IRule &);
	void addPostPlayRule(Rules::IRule &);

    std::list<Turn> &GetListOfTurn();

	Goban & getGoban() const;

protected:

private:

	std::list<Rules::IRule*> _prePlayRules;
	std::list<Rules::IRule*> _playRules;
	std::list<Rules::IRule*> _postPlayRules;
	std::list<Turn> _playedTurns;
	Goban & _goban;
	Turn _currentTurn;
	bool _gameFinished;
	Goban::PION_TYPE _winner;

};

#endif
