#ifndef _EACHINTURNRULE_H_
# define _EACHINTURNRULE_H_

#include "Referrer.h"
#include "Goban.h"
#include "IRule.h"

class EachInTurnRule : public IRule
{
public:
	EachInTurnRule(void);
	virtual ~EachInTurnRule(void);

	virtual void enable();
	virtual void disable();
	virtual bool isEnable() const;
	virtual std::string const & name() const;
	virtual bool execute(Referrer &, Goban::PION_TYPE, unsigned int, unsigned int);

private:
	bool _enable;
	Goban::PION_TYPE _lastPion;
};

#endif