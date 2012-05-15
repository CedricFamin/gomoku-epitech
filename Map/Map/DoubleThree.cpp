#include "Referrer.h"
#include "Goban.h"
#include "DoubleThree.h"

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

#include <iostream>
void print_case(unsigned long long int);


bool DoubleThree::execute(Referrer & r, Goban::PION_TYPE pion, unsigned int x, unsigned int y)
{
	int doublethree = 0;
	Goban::Case cCase = r.getGoban().GetMap()[y][x] >> Goban::HEADERSIZE;
	unsigned long long int double3[5][2] = 
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
				0x3 | Pattern<1,4,0x6>::mask | (Pattern<0,1,0x1>::value << (Goban::PATTERNSIZE * 4)),
			},
			{
				(pion | Pattern<1,4,0x6>::value) << (Goban::PATTERNSIZE * 4), 
				Pattern<0,1,0x1>::value | ((0x3 | Pattern<1,4,0x6>::mask) << (Goban::PATTERNSIZE * 4)),
			}
	};
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			if ((cCase & double3[j][1]) == double3[j][0])
			{
					
				std::cout << i << " " << j << std::endl;
				
			}
		}
		cCase >>= Goban::PATTERNSIZE;
	}

	return doublethree <= 1;
}