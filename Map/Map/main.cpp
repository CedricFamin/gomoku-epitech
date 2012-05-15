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

	play(referrer, Goban::BLACK, 2, 1);
	play(referrer, Goban::RED, 2, 3);
	play(referrer, Goban::BLACK, 2, 2);
	play(referrer, Goban::RED, 2, 0);
	play(referrer, Goban::BLACK, 4, 4);
	
	goban.Putin(Goban::RED, 0, 5);
	goban.Putin(Goban::RED, 1, 5);
	goban.Putin(Goban::RED, 4, 5);
	goban.Putin(Goban::RED, 3, 5);
	play(referrer, Goban::RED, 2, 6);

	//goban.Putin(Goban::RED, 0, 8);
	goban.Putin(Goban::BLACK, 1, 11);
	goban.Putin(Goban::BLACK, 2, 10);
	goban.Putin(Goban::BLACK, 4, 9);
	goban.Putin(Goban::BLACK, 4, 7);
	play(referrer, Goban::BLACK, 4, 8);
	
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
	system("pause");
	return 0;
}