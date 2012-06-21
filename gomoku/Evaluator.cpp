#include "Evaluator.h"


Evaluator::Evaluator(Goban const * g) : _base(g)
{
}


Evaluator::~Evaluator(void)
{
}

int Evaluator::operator()(Goban & g, Goban::PION_TYPE p)
{
	return 0;
}
