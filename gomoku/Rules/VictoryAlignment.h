#ifndef VICTORYALIGNMENT_H_
# define VICTORYALIGNMENT_H_

#include "IRule.h"
#include "Referrer.h"

namespace Rules
{
	class VictoryAlignment : public IRule
	{
	public:
		VictoryAlignment(void);
		virtual ~VictoryAlignment(void);
	
		void enableOptionalRule();
		void disableOptionalRule();
		virtual void enable();
		virtual void disable();
		virtual bool isEnable() const;
		virtual bool execute(Goban &, Goban::Turn &);

	private:
		bool _enable;
		bool _enableOptionalRule;
	};
}

#endif
