#ifndef DOUBLETHREE_H_
# define DOUBLETHREE_H_

#include "Goban.h"
#include "IRule.h"

namespace Rules
{
	class DoubleThree : public IRule
	{
	public:
		DoubleThree(void);
		virtual ~DoubleThree(void);
		
		virtual void enable();
		virtual void disable();
		virtual bool isEnable() const;
		virtual bool execute(Referrer &, Goban::PION_TYPE, unsigned int, unsigned int);

	private:
		bool _enable;
	};
}

#endif