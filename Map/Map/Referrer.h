#ifndef _REFERRER_H_
# define _REFERRER_H_

#include <list>
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
	
	void addPrePlayRule(IRule &);

	void addPlayRule(IRule &);

	void addPostPlayRule(IRule &);

	std::list<Turn> const & GetListOfTurn() const;

	Goban & getGoban() const;

protected:

private:

	std::list<IRule*> _prePlayRules;
	std::list<IRule*> _playRules;
	std::list<IRule*> _postPlayRules;
	std::list<Turn> _playedTurns;
	Goban & _goban;
	Turn _currentTurn;
	bool _gameFinished;
	Goban::PION_TYPE _winner;

};

#endif