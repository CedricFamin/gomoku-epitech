#ifndef _NOTEMPTYRULE_H_
# define _NOTEMPTYRULE_H_

#include "Goban.h"
#include "Referrer.h"
#include "IRule.h"

namespace Rules
{
	class NotEmptyRule :public IRule
	{
	public:
		NotEmptyRule(void);
		virtual ~NotEmptyRule(void);

		virtual void enable();
		virtual void disable();
		virtual bool isEnable() const;
		virtual bool execute(Goban &, Goban::Turn &);
		virtual IRule* clone() const;

	private:
		bool _enable;
	};
}

#endif

