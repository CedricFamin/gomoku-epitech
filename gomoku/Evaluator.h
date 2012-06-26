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
	int alignments(Goban::Case c, Goban &, unsigned int, unsigned int, Goban::PION_TYPE);

	Goban const * _base;
};

#endif