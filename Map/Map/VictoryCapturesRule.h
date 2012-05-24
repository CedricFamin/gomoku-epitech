#ifndef VICTORYCAPTURESRULE_H_
# define VICTORYCAPTURESRULE_H_


#include "IRule.h"
#include "TakingRules.h"

namespace Rules
{
	class VictoryCapturesRule : public IRule
	{
	public:
		VictoryCapturesRule(TakingRules const &);
		virtual ~VictoryCapturesRule(void);

		virtual void enable();
		virtual void disable();
		virtual bool isEnable() const;
		virtual bool execute(Referrer &, Goban::PION_TYPE, unsigned int, unsigned int);

	private:
		bool _enable;
		TakingRules const & _takingRule;
	};
}

#endif