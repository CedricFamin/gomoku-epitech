#ifndef _TAKINGRULE_H_
# define _TAKINGRULE_H_

#include "Goban.h"
#include "Referrer.h"
#include "IRule.h"

namespace Rules
{
	class TakingRules : public IRule
	{
	public:
		TakingRules(void);
		virtual ~TakingRules(void);

		virtual void enable();
		virtual void disable();
		virtual bool isEnable() const;
		virtual bool execute(Goban &, Goban::Turn &);
		virtual IRule* clone() const;

		std::stack<std::pair<unsigned int, unsigned int>> toDelete;

	private:
		bool _enable;
	};
}

#endif