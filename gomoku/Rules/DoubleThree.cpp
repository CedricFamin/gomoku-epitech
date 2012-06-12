#include "Referrer.h"
#include "Goban.h"
#include "DoubleThree.h"

#include "PatternIdentifier.h"

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

bool Check(Goban & g, unsigned int x, unsigned int y, int dir, int patternIndex, Goban::Case pattern, Goban::Case double3, Goban::PION_TYPE pion)
{
    const int moves[8][2] = {
        { 0,-1}, { 1, -1}, { 1, 0}, { 1, 1},
        {0, 1}, {-1, 1}, { -1,0}, { -1,-1}
    };
    static const int caseIndex[9] = {1,1,-1,1,-1,2,-2,1,2};
    unsigned int lx, ly;
    if (double3 && double3 != pattern)
        return false;
    else if (!double3)
    {
        lx = x + moves[dir][0];
        ly = y + moves[dir][1];
        if (!g.InBound(lx, ly))
            return false;
        if (g.GetMap()[ly][lx] & Goban::PIONMASK)
            return false;
    }
    else
    {
        lx = x + moves[dir][0] * caseIndex[patternIndex];
        ly = y + moves[dir][1] * caseIndex[patternIndex];
        if ((g.GetMap()[ly][lx] & Goban::PIONMASK) != pion)
            return false;
    }
    return true;
}

Goban::Case Double3(Goban & g, unsigned int x, unsigned int y, int dir, Goban::Case pattern1, Goban::Case pattern2, const Goban::Case patterns[9][2], Goban::PION_TYPE pion)
{
    int doubleThree = -1;
    for (int j = 0; j < 9; ++j)
    {
        if (Check(g, x, y, dir, j, pattern1, patterns[j][0], pion) &&
            Check(g, x, y, dir + 4, j, pattern2, patterns[j][1], pion))
        {
            if (doubleThree >= 0) return -2;
            doubleThree = j;
        }
    }
    return doubleThree;
}

bool DoubleThree::execute(Referrer & r, Goban::PION_TYPE pion, unsigned int x, unsigned int y)
{
    int doublethree = 0, result, subresult;
	Goban::Case cCase = r.getGoban().GetMap()[y][x] >> Goban::HEADERSIZE, subCase;
    static const Goban::Case double3[9][2] = {
        {Patterns::o_, Patterns::o_},
        {Patterns::oo_, 0},
        {0, Patterns::oo_},
        {Patterns::o_o_, 0},
        {0, Patterns::o_o_},
        {Patterns::_oo_, 0},
        {0, Patterns::_oo_},
        {Patterns::o_, Patterns::_o_},
        {Patterns::_o_, Patterns::o_}
	};
    static const int caseIndex[9][2] = {{ 1,-1}, { 1, 2}, {-1,-2}, { 1, 3}, {-1,-3}, { 2, 3}, {-2,-3}, { 1,-2}, {2, -1}};
    const int moves[4][2]            = {{ 0,-1}, { 1,-1}, { 1, 0}, { 1, 1}};
    Goban::Case pattern1, pattern2;
    unsigned int lx, ly;

    for (int dir = 0; dir < 4; ++dir)
	{
        pattern1 = cCase & Goban::PATTERNMASK;
        pattern2 = (cCase >> (Goban::PATTERNSIZE * 4)) & Goban::PATTERNMASK;
        result = Double3(r.getGoban(), x, y, dir, pattern1, pattern2, double3, pion);
        switch(result)
        {
        case -2: return false;
        case -1: break;
        default:
            if (++doublethree == 2) return false;
            for (int iCase = 0; iCase < 2; ++iCase)
            {
                lx = x + moves[dir][0] * caseIndex[result][iCase];
                ly = y + moves[dir][1] * caseIndex[result][iCase];
                subCase = r.getGoban().GetMap()[ly][lx] >> Goban::HEADERSIZE;
                for (int subDir = 0; subDir < 4; ++subDir)
                {
                    if (subDir != dir)
                    {
                        pattern1 = subCase & Goban::PATTERNMASK;
                        pattern2 = (subCase >> (Goban::PATTERNSIZE * 4)) & Goban::PATTERNMASK;
                        subresult = Double3(r.getGoban(), lx, ly, subDir, pattern1, pattern2, double3, pion);
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
