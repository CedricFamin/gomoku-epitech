#include <stdlib.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <windows.h>
#include <bitset>

#include "Goban.h"
#include "Referrer.h"
#include "EachInTurnRule.h"
#include "NotEmptyRule.h"
#include "TakingRules.h"
#include "VictoryCapturesRule.h"
#include "VictoryAlignment.h"
#include "DoubleThree.h"


void play(Referrer & r, Goban::PION_TYPE p, unsigned int x, unsigned int y)
{
	bool canplay =  r.CanPlay(p, x, y);
	std::cout << "Tour de jeu " << ((p==Goban::BLACK) ? "noir  ": "rouge ") << x << " " << y << " : " << canplay << std::endl;
	if (!canplay)
		return;
	r.Play();
	r.AfterPlay();
	
}

void print_case(unsigned long long int current)
{
	for (int i = 0; i <64; ++i)
	{
		std::cout << (current & 1) ? "1": "0";
		current >>= 1;
	}
}

int main(int ac, char **av)
{
	Goban goban;
	Rules::EachInTurnRule rule1;
	Rules::NotEmptyRule rule2;
	Rules::TakingRules rule3;
	Rules::VictoryCapturesRule rule4(rule3);
	Rules::VictoryAlignment rule5;
	Rules::DoubleThree rule6;

	rule5.enableOptionalRule();

	Referrer referrer(goban);
	//referrer.addPrePlayRule(rule1);
	referrer.addPrePlayRule(rule2);
	referrer.addPlayRule(rule3);
	referrer.addPostPlayRule(rule4);
	referrer.addPostPlayRule(rule5);
	referrer.addPrePlayRule(rule6);

	/*play(referrer, Goban::BLACK, 2, 1);
	play(referrer, Goban::RED, 2, 3);
	play(referrer, Goban::BLACK, 2, 2);
	play(referrer, Goban::RED, 2, 0);
	play(referrer, Goban::BLACK, 4, 4);
	
	goban.Putin(Goban::BLACK, 0, 4);
	goban.Putin(Goban::RED, 0, 6);
	goban.Putin(Goban::RED, 0, 5);
	goban.Putin(Goban::RED, 1, 5);
	goban.Putin(Goban::RED, 4, 5);
	//goban.Putin(Goban::RED, 5, 5);
	goban.Putin(Goban::RED, 3, 5);
	play(referrer, Goban::RED, 2, 5);

	//goban.Putin(Goban::RED, 0, 8);
	goban.Putin(Goban::BLACK, 2, 9);
	goban.Putin(Goban::BLACK, 1, 9);
	goban.Putin(Goban::BLACK, 4, 9);
	//goban.Putin(Goban::BLACK, 4, 7);
	play(referrer, Goban::BLACK, 4, 8);
	play(referrer, Goban::RED, 1, 7);
	play(referrer, Goban::BLACK, 0, 7);*/

	/*goban.Putin(Goban::RED, 8, 8);
	goban.Putin(Goban::BLACK, 8, 7);
	goban.Putin(Goban::BLACK, 8, 6);

	goban.Putin(Goban::RED, 5, 8);
	goban.Putin(Goban::BLACK, 6, 7);
	goban.Putin(Goban::BLACK, 7, 6);

	goban.Putin(Goban::RED, 5, 5);
	goban.Putin(Goban::BLACK, 6, 5);
	goban.Putin(Goban::BLACK, 7, 5);

	goban.Putin(Goban::RED, 5, 2);
	goban.Putin(Goban::BLACK, 6, 3);
	goban.Putin(Goban::BLACK, 7, 4);

	goban.Putin(Goban::RED, 8, 2);
	goban.Putin(Goban::BLACK, 8, 3);
	goban.Putin(Goban::BLACK, 8, 4);

	goban.Putin(Goban::RED, 11, 2);
	goban.Putin(Goban::BLACK, 10, 3);
	goban.Putin(Goban::BLACK, 9, 4);

	goban.Putin(Goban::RED, 11, 5);
	goban.Putin(Goban::BLACK, 10, 5);
	goban.Putin(Goban::BLACK, 9, 5);

	goban.Putin(Goban::RED, 11, 8);
	goban.Putin(Goban::BLACK, 10, 7);
	goban.Putin(Goban::BLACK, 9, 6);
	play(referrer, Goban::RED, 8, 5);*/
	unsigned int x,  y;
	/*goban.Putin(Goban::RED, 7, 6);
	goban.Putin(Goban::RED, 8, 7);
	goban.Putin(Goban::RED, 9, 8);
	goban.Putin(Goban::RED, 12, 8);
	play(referrer, Goban::RED, 10, 8);*/
	while (!referrer.GameFinished())
	{
		
		std::cin >> x >> y;
		play(referrer, Goban::RED, x, y);
		//play(referrer, Goban::BLACK, x, y);
		for (unsigned int y = 0; y < 19; y++)
		{
			for (unsigned int x = 0; x < 19; x++)
			{
				switch (goban.GetMap()[y][x] & Goban::PIONMASK)
				{
				case 0x1:
					std::cout << "x ";
					break;
				case 0x3:
					std::cout << "o ";
					break;
				default:
					std::cout << ". ";
				}
			}
			std::cout << std::endl;
		}
	}
	std::cout << referrer.GameFinished() << " " << referrer.Winner() << std::endl;
	system("pause");
	return 0;
}