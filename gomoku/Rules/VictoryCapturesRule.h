#ifndef VICTORYCAPTURESRULE_H_
# define VICTORYCAPTURESRULE_H_


#include "IRule.h"
#include "TakingRules.h"

namespace Rules
{
	class VictoryCapturesRule : public IRule
	{
	public:
		VictoryCapturesRule();
		virtual ~VictoryCapturesRule(void);

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