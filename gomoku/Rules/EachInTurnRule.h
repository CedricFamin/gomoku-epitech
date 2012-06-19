#ifndef _EACHINTURNRULE_H_
# define _EACHINTURNRULE_H_

#include "Referrer.h"
#include "Goban.h"
#include "IRule.h"

namespace Rules
{
	class EachInTurnRule : public IRule
	{
	public:
		EachInTurnRule(void);
		virtual ~EachInTurnRule(void);

		virtual void enable();
		virtual void disable();
		virtual bool isEnable() const;
		virtual bool execute(Goban &, Goban::PION_TYPE, unsigned int, unsigned int);

	private:
		bool _enable;
		Goban::PION_TYPE _lastPion;
	};
}
#endif