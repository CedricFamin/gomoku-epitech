#ifndef LASTTAKING_H_
# define LASTTACKING_H_

#include "IRule.h"

class LastTaking :
	public IRule
{
public:
	LastTaking(void);
	virtual ~LastTaking(void);

	virtual void enable();
	virtual void disable();
	virtual bool isEnable() const;
	virtual std::string const & name() const;
	virtual bool execute(Referrer &, Goban::PION_TYPE, unsigned int, unsigned int);

private:
	bool _enable;
};

#endif