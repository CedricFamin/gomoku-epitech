#include "Referrer.h"
#include "Goban.h"
#include "DoubleThree.h"

#include "PatternIdentifier.h"
#include "GobanIterator.h"

using namespace Rules;

DoubleThree::DoubleThree(void) : _enable(true)
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

bool Check(Goban & g, unsigned int x, unsigned int y, int dir, Goban::Case pattern, Goban::Case double3, int caseIndex, Goban::PION_TYPE pion)
{
    unsigned int lx, ly;
    if (double3 && double3 != pattern)
        return false;
    else if (!double3)
    {
        lx = x + GobanIterator::direction[dir][0];
        ly = y + GobanIterator::direction[dir][1];
        if (!g.InBound(lx, ly))
            return false;
        if (g[ly][lx] & Goban::PIONMASK)
            return false;
    }
    else
    {
        lx = x + GobanIterator::direction[dir][0] * caseIndex;
        ly = y + GobanIterator::direction[dir][1] * caseIndex;
        if ((g[ly][lx] & Goban::PIONMASK) != pion)
            return false;
    }
    return true;
}

Goban::Case Double3(Goban & g, unsigned int x, unsigned int y, int dir, Goban::Case pattern1, Goban::Case pattern2, const Goban::Case patterns[9][2], Goban::PION_TYPE pion)
{
    int doubleThree = -1;
	static const int caseIndex[12][2] = {{ 1,1}, { 1,1}, { 1,1}, { 1,1}, { 1, 2}, {1,2}, { 1, 3}, {-1,3}, { 2, 3}, {2,3}, { 1,2}, {2, 1}};

    for (int j = 0; j < 12; ++j)
    {
        if (Check(g, x, y, dir, pattern1, patterns[j][0], caseIndex[j][0], pion) &&
            Check(g, x, y, dir + 4, pattern2, patterns[j][1], caseIndex[j][1], pion))
        {
            if (doubleThree >= 0) return -2;
            doubleThree = j;
        }
    }
    return doubleThree;
}

bool DoubleThree::execute(Goban & g, Goban::Turn & turn)
{
    int doublethree = 0, result, subresult;
	Goban::Case cCase = g[turn.y][turn.x] >> Goban::HEADERSIZE, subCase;
    static const Goban::Case double3[12][2] = {
        {Patterns::o_, Patterns::o_},
		{Patterns::o_, Patterns::o_o_},
		{Patterns::o_o_, Patterns::o_},
		{Patterns::o_o_, Patterns::o_o_},
        {Patterns::oo_, 0},
        {0, Patterns::oo_},
        {Patterns::o_o_, 0},
        {0, Patterns::o_o_},
        {Patterns::_oo_, 0},
        {0, Patterns::_oo_},
        {Patterns::o_, Patterns::_o_},
        {Patterns::_o_, Patterns::o_}
	};
    static const int caseIndex[12][2] = {{ 1,-1}, { 1,-1}, { 1,-1}, { 1,-1}, { 1, 2}, {-1,-2}, { 1, 3}, {-1,-3}, { 2, 3}, {-2,-3}, { 1,-2}, {2, -1}};
    const int moves[4][2]            = {{ 0,-1}, { 1,-1}, { 1, 0}, { 1, 1}};
    Goban::Case pattern1, pattern2;
    unsigned int lx, ly;

    for (int dir = 0; dir < 4; ++dir)
	{
        pattern1 = cCase & Goban::PATTERNMASK;
        pattern2 = (cCase >> (Goban::PATTERNSIZE * 4)) & Goban::PATTERNMASK;
        result = Double3(g, turn.x, turn.y, dir, pattern1, pattern2, double3, turn.pion);
        switch(result)
        {
        case -2: return false;
        case -1: break;
        default:
            if (++doublethree == 2) return false;
            for (int iCase = 0; iCase < 2; ++iCase)
            {
                lx = turn.x + GobanIterator::direction[dir][0] * caseIndex[result][iCase];
                ly = turn.y + GobanIterator::direction[dir][1] * caseIndex[result][iCase];
                subCase = g[ly][lx] >> Goban::HEADERSIZE;
                for (int subDir = 0; subDir < 4; ++subDir)
                {
                    if (subDir != dir)
                    {
                        pattern1 = subCase & Goban::PATTERNMASK;
                        pattern2 = (subCase >> (Goban::PATTERNSIZE * 4)) & Goban::PATTERNMASK;
                        subresult = Double3(g, lx, ly, subDir, pattern1, pattern2, double3, turn.pion);
                        if (subresult != -1) return false;
                    }
                    subCase >>= Goban::PATTERNSIZE;
                }
            }
        }
        cCase >>= Goban::PATTERNSIZE;
	}

    return true;
}
