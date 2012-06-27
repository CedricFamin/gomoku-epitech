#ifndef EVALUATOR_H_
# define EVALUATOR_H_

#include "Goban.h"

class Evaluator
{
public:
	Evaluator(Goban const *);
	~Evaluator(void);

	int operator()(Goban & g, Goban::PION_TYPE p);
private:
	int influence(Goban::Case c, Goban &, unsigned int, unsigned int, Goban::PION_TYPE);

	struct Eval
	{
		bool eval;
		char counter:3;
		char align:4;
	};

	Eval _eval[361][4];

	Goban const * _base;
};

#endif