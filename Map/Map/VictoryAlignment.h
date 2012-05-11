#ifndef VICTORYALIGNMENT_H_
# define VICTORYALIGNMENT_H_

#include "IRule.h"
#include "Referrer.h"

class VictoryAlignment :
	public IRule
{
public:
	VictoryAlignment(void);
	virtual ~VictoryAlignment(void);
	
	virtual void enable();
	virtual void disable();
	virtual bool isEnable() const;
	virtual std::string const & name() const;
	virtual bool execute(Referrer &, Goban::PION_TYPE, unsigned int, unsigned int);

private:
	bool _enable;
};

#endif