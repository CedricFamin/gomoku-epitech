#include <stdlib.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <windows.h>

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
	for (unsigned int y = 0; y < 19; y++)
	{
		for (unsigned int x = 0; x < 19; x++)
		{
			switch (r.getGoban().GetMap()[y][x].color)
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

int main(int ac, char **av)
{
	Goban goban;
	EachInTurnRule rule1;
	NotEmptyRule rule2;
	TakingRules rule3;
	VictoryCapturesRule rule4(rule3);
	VictoryAlignment rule5;
	DoubleThree rule6;

	Referrer referrer(goban);
	referrer.addPrePlayRule(rule1);
	referrer.addPrePlayRule(rule2);
	referrer.addPlayRule(rule3);
	referrer.addPostPlayRule(rule4);
	referrer.addPostPlayRule(rule5);
	referrer.addPrePlayRule(rule6);

	play(referrer, Goban::RED, 9, 2);
	play(referrer, Goban::BLACK, 7, 8);
	play(referrer, Goban::RED, 10, 2);
	play(referrer, Goban::BLACK, 6, 6);
	play(referrer, Goban::RED, 11, 3);
	play(referrer, Goban::BLACK, 9, 5);
	play(referrer, Goban::RED, 11, 4);
	play(referrer, Goban::BLACK, 11, 11);

	play(referrer, Goban::RED, 11, 2);
	play(referrer, Goban::RED, 11, 14);
	play(referrer, Goban::BLACK, 11, 1);
	play(referrer, Goban::RED, 11, 2);

	play(referrer, Goban::RED, 1, 0);
	play(referrer, Goban::BLACK, 2, 0);
	play(referrer, Goban::RED, 1, 1);
	play(referrer, Goban::BLACK, 3, 0);
	play(referrer, Goban::RED, 4, 0);
	play(referrer, Goban::BLACK, 0, 0);
	play(referrer, Goban::RED, 2, 2);
	play(referrer, Goban::BLACK, 3, 3);
	play(referrer, Goban::RED, 2, 0);
	play(referrer, Goban::BLACK, 1, 1);
	play(referrer, Goban::RED, 3, 0);
	play(referrer, Goban::BLACK, 2, 2);

	play(referrer, Goban::RED, 10, 10);
	play(referrer, Goban::BLACK, 2, 1);
	
	
	play(referrer, Goban::RED, 9, 9);
	play(referrer, Goban::BLACK, 4, 4);
	
	std::cout << "Finished : " << referrer.GameFinished() << " winner : " << ((referrer.Winner()==Goban::BLACK) ? "noir  ": "rouge ") << std::endl;
	
	system("pause");
	return 0;
}