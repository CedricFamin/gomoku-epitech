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

	int __o_Pattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int __ooPattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int __oo_Pattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int __ooxPattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int __oooPattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int _o_Pattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int _o_o_Pattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int _oxPattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int _ooPattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int _oo_Pattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int _ooxPattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int _oooPattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int _ooo_Pattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int _oooxPattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int _ooooPattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
	int _emptyPattern(Goban &, unsigned int, unsigned int,int, Goban::PION_TYPE);
};

#endif