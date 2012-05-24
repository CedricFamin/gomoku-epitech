#ifndef _IRULE_H_
# define _IRULE_H_

#include <string>

#include "Goban.h"

class Referrer;

namespace Rules
{
	class IRule
	{
	public:
		IRule(void) { }
		virtual ~IRule(void) { }

		virtual void enable() = 0;
		virtual void disable() = 0;
		virtual bool isEnable() const = 0;
		virtual bool execute(Referrer &, Goban::PION_TYPE, unsigned int, unsigned int) = 0;
	};
}

#endif