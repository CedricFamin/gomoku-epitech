#include "Referrer.h"
#include "Goban.h"
#include "DoubleThree.h"

using namespace Rules;

DoubleThree::DoubleThree(void)
{
}


DoubleThree::~DoubleThree(void)
{
}

void DoubleThree::enable()
{
	this->_enable = true;
}

void DoubleThree::disable()
{
	this->_enable = false;
}

bool DoubleThree::isEnable() const
{
	return this->_enable;
}

void print_case(unsigned long long int current);
#include <iostream>

bool DoubleThree::execute(Referrer & r, Goban::PION_TYPE pion, unsigned int x, unsigned int y)
{
	int doublethree = 0;
	Goban::Case cCase = r.getGoban().GetMap()[y][x] >> Goban::HEADERSIZE, subCase;
	int double3move[9][2] = { {1, 2}, {-1, -2}, {1, -1}, {3, 2}, {-3, 2}, {1, 3}, {-1, -3}, {1, -2}, {2, -1} };
	int direction[4][2] = { {0, -1}, {1,-1}, {1,0}, {1,1} };
	unsigned long long int double3[9][2] = 
		{
			{
				pion | Pattern<1,3,0x3>::value, 
				0x3 | Pattern<1,3,0x3>::value | ((0x0 | Pattern<0,1,0x1>::value)  << (Goban::PATTERNSIZE * 4))
			},
			{
				(pion | Pattern<1,3,0x3>::value) << (Goban::PATTERNSIZE * 4),
				0x0 | Pattern<0,1,0x1>::value | ((0x3 | Pattern<1,3,0x3>::value)  << (Goban::PATTERNSIZE * 4))
			},
			{
				pion | Pattern<1,1,0x1>::value | ((pion | Pattern<1,1,0x1>::value) << (Goban::PATTERNSIZE * 4)),
				0x3 | Pattern<1,2,0x3>::value | ((0x3 | Pattern<1,2,0x3>::value) << (Goban::PATTERNSIZE * 4))
			},
			{
				pion | Pattern<1,4,0x6>::value, 
				Pattern<1,4,0x6>::mask | (Pattern<0,1,0x1>::value << (Goban::PATTERNSIZE * 4)),
			},
			{
				(pion | Pattern<1,4,0x6>::value) << (Goban::PATTERNSIZE * 4), 
				Pattern<0,1,0x1>::value | (Pattern<1,4,0x6>::mask << (Goban::PATTERNSIZE * 4)),
			},
			{
				pion | Pattern<1,4,0x5>::value, 
				Pattern<1,4,0x5>::mask | (Pattern<0,1,0x1>::value << (Goban::PATTERNSIZE * 4)),
			},
			{
				(pion | Pattern<1,4,0x5>::value) << (Goban::PATTERNSIZE * 4), 
				Pattern<0,1,0x1>::value | (Pattern<1,4,0x5>::mask << (Goban::PATTERNSIZE * 4)),
			},
			{
				pion | Pattern<1,1,0x1>::value | ((pion | Pattern<1,2,0x2>::value) << (Goban::PATTERNSIZE * 4)), 
				Pattern<1,1,0x1>::mask | (Pattern<1,2,0x2>::mask << (Goban::PATTERNSIZE * 4)),
			},
			{
				pion | Pattern<1,2,0x2>::value | ((pion | Pattern<1,1,0x1>::value) << (Goban::PATTERNSIZE * 4)), 
				Pattern<1,2,0x2>::mask | (Pattern<1,1,0x1>::mask << (Goban::PATTERNSIZE * 4)),
			}
	};

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			if ((cCase & double3[j][1]) == double3[j][0])
			{
				if (++doublethree == 2) return false;
				for (int caseIndex = 0; caseIndex < 2; ++caseIndex)
				{
					subCase = r.getGoban().GetMap()[double3move[j][caseIndex] * direction[i][1] + y][double3move[j][caseIndex] * direction[i][0] + x] >> Goban::HEADERSIZE;
					for (int d = 0; d < 5; ++d)
					{
						if (d != i)
							for (int k = 0; k < 9; ++k)
							{
								if ((subCase & double3[k][1]) == double3[k][0])
									return false;
							}
						subCase >>= Goban::PATTERNSIZE;
					}
				}
			}
		}
		cCase >>= Goban::PATTERNSIZE;
	}

	return true;
}