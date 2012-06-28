#ifndef _REFERRER_H_
# define _REFERRER_H_

#include <list>
#include <utility>
#include "Goban.h"
#include "Rules/IRule.h"

class Referrer
{
public:
	Referrer();
	Referrer(Referrer const &);
	~Referrer(void);

	bool operator()(Goban & g, Goban::PION_TYPE, unsigned int, unsigned int, bool = true);
	void addPrePlayRule(Rules::IRule &);
	void addPlayRule(Rules::IRule &);
	void addPostPlayRule(Rules::IRule &);
protected:

private:

	std::list<Rules::IRule*> _prePlayRules;
	std::list<Rules::IRule*> _playRules;
	std::list<Rules::IRule*> _postPlayRules;
	std::list<Rules::IRule*>::iterator _beginPrePlay, _endPrePlay;
	std::list<Rules::IRule*>::iterator _beginPlay, _endPlay;
	std::list<Rules::IRule*>::iterator _beginPostPlay, _endPostPlay;
};

#endif
