#include <iostream>
#include <algorithm>

#include "VictoryAlignment.h"
#include "PatternIdentifier.h"

using namespace Rules;

VictoryAlignment::VictoryAlignment(void) : _enable(true), _enableOptionalRule(true)
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

int VictoryAlignment::GetAlign(Goban & g, unsigned int x, unsigned int y, Goban::Case pattern, Goban::PION_TYPE pion)
{
    int maxAlign = 0;

    if (g.InBound(x, y) && (g.GetMap()[y][x] & Goban::PIONMASK) == pion)
    {
        maxAlign += 1;
        maxAlign += (pattern >= Patterns::align_2) ? 1: 0;
        maxAlign += (pattern >= Patterns::align_3) ? 1: 0;
        maxAlign += (pattern >= Patterns::align_4) ? 1: 0;
    }
    return maxAlign;
}

bool winningAlignment(Goban & g, int size, int dir, int x, int y, Goban::PION_TYPE pion, bool && breaked = true)
{
	const int moves[4][2] = { { 0,-1}, { 1, -1}, { 1, 0}, { 1, 1} };
	int align = 0;
	Goban::Case aCase;

	for (int i = 0; i <= size; ++i)
	{
		aCase = g.GetMap()[y][x];
		if ((aCase & Goban::PIONMASK) != pion)
		{
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
					Goban::Case pattern1 = aCase & Goban::PATTERNMASK;
					Goban::Case pattern2 = (aCase >> (Goban::PATTERNSIZE * 4)) & Goban::PATTERNMASK;
					unsigned int ux = x - moves[j][0];
					unsigned int uy = y - moves[j][1];
					unsigned int dx = x + moves[j][0];
					unsigned int dy = y + moves[j][1];
					Goban::Case upCase   = (g.InBound(ux, uy)) ? g.GetMap()[uy][ux] & Goban::PIONMASK : -1;
					Goban::Case downCase = (g.InBound(dx, dy)) ? g.GetMap()[dy][dx] & Goban::PIONMASK : -1;
					if ((pattern1 == Patterns::ox && upCase == 0) ||
						(pattern2 == Patterns::ox && downCase == 0))
					{
						align = -1;
						break;
					}
					if (upCase > 0 && downCase > 0 &&
						((upCase == pion && downCase != pion) ||
						(upCase != pion && downCase == pion)))
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

bool VictoryAlignment::execute(Referrer & r, Goban::PION_TYPE pion, unsigned int x, unsigned int y)
{
    const int direction[8][2] = {
        { 0,-1}, { 1, -1}, { 1, 0}, { 1, 1},
        {0, 1}, {-1, 1}, { -1,0}, { -1,-1}
    };

    Goban::Case cCase = r.getGoban().GetMap()[y][x] >> Goban::HEADERSIZE;
    Goban::Case pattern1, pattern2;
	unsigned int lx, ly;

	this->_aligments.remove_if([&r](Align & align)->bool
	{
		bool breaked = false;
		if (winningAlignment(r.getGoban(), align.size, align.dir, align.x, align.y, align.pion, std::forward<bool>(breaked)))
		{
			r.setGameFinished(true);
			r.setWinner(align.pion);
		}
		return breaked;
	});
	if (r.GameFinished()) return true;
    for (int i = 0; i < 4; ++i)
    {
        int maxAlign = 0;
        pattern1 = cCase & Goban::PATTERNMASK;
        pattern2 = (cCase >> (Goban::PATTERNSIZE * 4)) & Goban::PATTERNMASK;
        maxAlign += GetAlign(r.getGoban(), x+direction[i][0], y+direction[i][1], pattern1, pion);
		lx = x + direction[i][0] * maxAlign;
		ly = y + direction[i][1] * maxAlign;
        maxAlign += GetAlign(r.getGoban(), x-direction[i][0], y-direction[i][1], pattern2, pion);
		if (maxAlign >= 4)
        {
			if (winningAlignment(r.getGoban(), maxAlign, i, lx, ly, pion))
			{
				r.setGameFinished(true);
				r.setWinner(pion);
				return true;
			}
			else
			{
				Align align;
				align.x = lx;
				align.y = ly;
				align.dir = i;
				align.size = maxAlign;
				align.pion = pion;
				this->_aligments.push_back(align);
			}
        }
        cCase >>= Goban::PATTERNSIZE;
    }
	return false;
}


