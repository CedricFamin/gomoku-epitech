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
	Goban const * _base;
};

#endif