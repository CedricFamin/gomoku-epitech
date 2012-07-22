#include <iostream>
#include <algorithm>

#include "VictoryAlignment.h"
#include "PatternIdentifier.h"

using namespace Rules;

VictoryAlignment::VictoryAlignment(void) : _enable(true), _enableOptionalRule(false)
{
}


VictoryAlignment::~VictoryAlignment(void)
{
}

void VictoryAlignment::enableOptionalRule()
{
	this->_enableOptionalRule = true;
}

void VictoryAlignment::disableOptionalRule()
{
	this->_enableOptionalRule = false;
}

void VictoryAlignment::enable()
{
	this->_enable = true;
}

void VictoryAlignment::disable()
{
	this->_enable = false;
}

bool VictoryAlignment::isEnable() const
{
	return this->_enable;
}

bool winningAlignment(Goban & g, int size, int dir, int x, int y, Goban::PION_TYPE pion, bool && breaked = true)
{
	const int moves[4][2] = { { 0,-1}, { 1, -1}, { 1, 0}, { 1, 1} };
	int align = 0;
	Goban::Case aCase;
	Patterns::PatternInfos pInfos1, pInfos2;


	for (int i = 0; i <= size; ++i)
	{
		aCase = g[y][x];
		if ((aCase & Goban::PIONMASK) != pion)
		{
			align = -1;
			if (size - i < 4)
			{
				breaked = true;
				return false;
			}
		}
		else
		{
			aCase >>=  Goban::HEADERSIZE;
			for (int j = 0; j < 4; ++j)
			{
				if (j != dir)
				{
					pInfos1 = Patterns::GetPatternInfos(aCase & Goban::PATTERNMASK);
					pInfos2 = Patterns::GetPatternInfos((aCase >> (Goban::PATTERNSIZE * 4)) & Goban::PATTERNMASK);
					unsigned int ux = x - moves[j][0];
					unsigned int uy = y - moves[j][1];
					unsigned int dx = x + moves[j][0];
					unsigned int dy = y + moves[j][1];
					Goban::Case upCase   = (g.InBound(ux, uy)) ? g[uy][ux] & Goban::PIONMASK : -1;
					Goban::Case downCase = (g.InBound(dx, dy)) ? g[dy][dx] & Goban::PIONMASK : -1;
					if (((pInfos1.pattern == Patterns::ox && upCase == 0 && downCase == pion) ||
                        (pInfos2.pattern == Patterns::ox && downCase == 0 && upCase == pion)) ||
						(upCase > 0 && downCase > 0 &&
						((upCase == pion && pInfos2.expand > 0 && pInfos2.align == 1 && downCase != pion) ||
						(upCase != pion && pInfos1.expand > 0 && pInfos1.align == 1 && downCase == pion))))
					{
						align = -1;
						break;
					}
				}
				aCase >>= Goban::PATTERNSIZE;
			}
		}
		x -= moves[dir][0];
		y -= moves[dir][1];
		if (++align == 5) return true;
	}
	return false;
}

bool VictoryAlignment::execute(Goban & g, Goban::Turn & turn)
{
    const int direction[8][2] = {
        { 0,-1}, { 1, -1}, { 1, 0}, { 1, 1},
        {0, 1}, {-1, 1}, { -1,0}, { -1,-1}
    };

    Goban::Case cCase = g[turn.y][turn.x] >> Goban::HEADERSIZE;
    Goban::Case pattern1, pattern2;
	unsigned int lx, ly;
	Patterns::PatternInfos pInfos;

	this->alignments.remove_if([&g](Goban::Align & align)->bool
	{
		bool breaked = false;
		if (winningAlignment(g, align.size, align.dir, align.x, align.y, align.pion, std::forward<bool>(breaked)))
		{
			g.setGameFinished(true);
			g.setWinner(align.pion);
		}
		return breaked;
	});
	if (g.gameFinished()) return true;
    for (int i = 0; i < 4; ++i)
    {
        int maxAlign = 0;
        pattern1 = cCase & Goban::PATTERNMASK;
        pattern2 = (cCase >> (Goban::PATTERNSIZE * 4)) & Goban::PATTERNMASK;
		pInfos = Patterns::GetPatternInfos(pattern1);
		maxAlign += pInfos.align;
		lx = turn.x + direction[i][0] * maxAlign;
		ly = turn.y + direction[i][1] * maxAlign;
		pInfos = Patterns::GetPatternInfos(pattern2);
		maxAlign += pInfos.align;
		if (maxAlign >= 4)
        {
			if (winningAlignment(g, maxAlign, i, lx, ly, turn.pion))
			{
				g.setGameFinished(true);
				g.setWinner(turn.pion);
				return true;
			}
			else
			{
				Goban::Align align;
				align.x = lx;
				align.y = ly;
				align.dir = i;
				align.size = maxAlign;
				align.pion = turn.pion;
				this->alignments.push_back(align);
			}
        }
        cCase >>= Goban::PATTERNSIZE;
    }
	return false;
}

IRule* VictoryAlignment::clone() const
{
	VictoryAlignment * va = new VictoryAlignment();
	va->alignments = this->alignments;
	return va;
}