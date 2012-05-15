#ifndef _TAKINGRULE_H_
# define _TAKINGRULE_H_

#include "Goban.h"
#include "Referrer.h"
#include "IRule.h"

class TakingRules : public IRule
{
public:
	TakingRules(void);
	virtual ~TakingRules(void);

	virtual void enable();
	virtual void disable();
	virtual bool isEnable() const;
	virtual bool execute(Referrer &, Goban::PION_TYPE, unsigned int, unsigned int);

	int const * getCaptures() const;

private:
	bool _enable;
	int  _capture[2];
};

#endif