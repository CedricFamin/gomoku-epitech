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
		virtual bool execute(Referrer &, Goban::PION_TYPE, unsigned int, unsigned int);

	private:
		bool checkAlign(Referrer & r);
		struct Align
		{
			unsigned int x;
			unsigned int y;
			unsigned int dir;
			Goban::PION_TYPE pion;
		};
		bool _enable;
		bool _enableOptionalRule;
		std::list<Align> _aligments;
	};
}

#endif