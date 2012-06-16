#include "Goban.h"

#include <iterator>
#include <QDebug>
#include "GobanIterator.h"
#include "PatternIdentifier.h"

#define MIN(x, y) ((x < y) ? x : y)

Goban::Goban() : _width(19), _height(19), _gameFinished(false)
{
	for (unsigned int i = 0; i < _width * _height; i++)
    {
        _map[i] = 0;
    }
	_deletedStone[0] = 0;
	_deletedStone[1] = 0;
}

Goban::Goban(Goban const & g) : _width(g._width), _height(g._height)
{
	_gameFinished = g._gameFinished;
	_deletedStone[0] = g._deletedStone[0];
	_deletedStone[1] = g._deletedStone[1];
	memcpy(_map,g._map, sizeof(*_map) * 19 * 19);
}

Goban::~Goban(void)
{
}

Goban::Case * Goban::operator[](unsigned int i)
{
	return this->_map + i * 19;
}

template<unsigned int p>
struct ColorSuitToPattern { };
template<> struct ColorSuitToPattern<Patterns::ox>   { enum values { pattern = Patterns::ox,   mask = 0xF,  v1 = 0x9,  v2 = 0x6 }; };
template<> struct ColorSuitToPattern<Patterns::o_>   { enum values { pattern = Patterns::o_,   mask = 0xF,  v1 = 0x2,  v2 = 0x1 }; };

template<> struct ColorSuitToPattern<Patterns::oo>   { enum values { pattern = Patterns::oo,   mask = 0x3F, v1 = 0x35, v2 = 0x3A }; };
template<> struct ColorSuitToPattern<Patterns::oo_>  { enum values { pattern = Patterns::oo_,  mask = 0x3F, v1 = 0x5,  v2 = 0xA }; };
template<> struct ColorSuitToPattern<Patterns::oox>  { enum values { pattern = Patterns::oox,  mask = 0x3F, v1 = 0x25, v2 = 0x1A }; };
template<> struct ColorSuitToPattern<Patterns::_o_>  { enum values { pattern = Patterns::_o_,  mask = 0x3F, v1 = 0x4,  v2 = 0x8 }; };

template<> struct ColorSuitToPattern<Patterns::_oo>  { enum values { pattern = Patterns::_oo,  mask = 0xFF, v1 = 0xD4, v2 = 0xE8 }; };
template<> struct ColorSuitToPattern<Patterns::_oo_> { enum values { pattern = Patterns::_oo_, mask = 0xFF, v1 = 0x14, v2 = 0x28 }; };
template<> struct ColorSuitToPattern<Patterns::_oox> { enum values { pattern = Patterns::_oox, mask = 0xFF, v1 = 0x94, v2 = 0x68 }; };
template<> struct ColorSuitToPattern<Patterns::_ooo> { enum values { pattern = Patterns::_ooo, mask = 0xFF, v1 = 0x54, v2 = 0xA8 }; };
template<> struct ColorSuitToPattern<Patterns::ooo>  { enum values { pattern = Patterns::ooo,  mask = 0xFF, v1 = 0xD5, v2 = 0xEA }; };
template<> struct ColorSuitToPattern<Patterns::ooo_> { enum values { pattern = Patterns::ooo_, mask = 0xFF, v1 = 0x15, v2 = 0x2A }; };
template<> struct ColorSuitToPattern<Patterns::ooox> { enum values { pattern = Patterns::ooox, mask = 0xFF, v1 = 0x95, v2 = 0x6A }; };
template<> struct ColorSuitToPattern<Patterns::oooo> { enum values { pattern = Patterns::oooo, mask = 0xFF, v1 = 0x55, v2 = 0xAA }; };
template<> struct ColorSuitToPattern<Patterns::o_o_> { enum values { pattern = Patterns::o_o_, mask = 0xFF, v1 = 0x22, v2 = 0x11 }; };

template<unsigned int p>
inline bool MatchPattern(unsigned int pattern)
{
	if ((ColorSuitToPattern<p>::mask & pattern) == ColorSuitToPattern<p>::v1)
		return true;
	if ((ColorSuitToPattern<p>::mask & pattern) == ColorSuitToPattern<p>::v2)
		return true;
	return false;
}

template<int direction>
inline void updatePattern(Goban & g, unsigned int x, unsigned int y)
{
    unsigned int shift = ((direction+4) % 8) * Goban::PATTERNSIZE + Goban::HEADERSIZE;
    Goban::Case cases = 0;
    unsigned int decal = 6;
    Goban::Case pattern;
    unsigned int lx = x - Padding<direction,3>::x, ly = y - Padding<direction,3>::y;

    for (int i = 0; i < 7; ++i)
	{
		 cases <<= 2;
		if (!g.InBound(lx, ly))
			cases |= 0x3;
		else
			cases |= g[ly][lx] & Goban::PIONMASK;
        lx += Padding<direction,1>::x;
        ly += Padding<direction,1>::y;
	}
    lx = x;
    ly = y;
    for (int i = 0; i < 4; ++i)
	{
        lx += Padding<direction,1>::x;
        ly += Padding<direction,1>::y;
        if (!g.InBound(lx, ly))
        {
            decal -= 2;
            continue;
        }

        pattern = cases >> decal;
        g[ly][lx] &= ~(Goban::PATTERNMASK << shift);
		if      (MatchPattern<Patterns::o_o_>(pattern)) g[ly][lx] |= (Goban::Case)Patterns::o_o_ << shift;
		else if (MatchPattern<Patterns::oooo>(pattern)) g[ly][lx] |= (Goban::Case)Patterns::oooo << shift;
		else if (MatchPattern<Patterns::ooox>(pattern)) g[ly][lx] |= (Goban::Case)Patterns::ooox << shift;
		else if (MatchPattern<Patterns::ooo_>(pattern)) g[ly][lx] |= (Goban::Case)Patterns::ooo_ << shift;
		else if (MatchPattern<Patterns::ooo>(pattern))  g[ly][lx] |= (Goban::Case)Patterns::ooo << shift;
        else if (MatchPattern<Patterns::_ooo>(pattern)) g[ly][lx] |= (Goban::Case)Patterns::_ooo << shift;
		else if (MatchPattern<Patterns::_oox>(pattern)) g[ly][lx] |= (Goban::Case)Patterns::_oox << shift;
		else if (MatchPattern<Patterns::_oo_>(pattern)) g[ly][lx] |= (Goban::Case)Patterns::_oo_ << shift;
		else if (MatchPattern<Patterns::_oo>(pattern))  g[ly][lx] |= (Goban::Case)Patterns::_oo << shift;
		else if (MatchPattern<Patterns::_o_>(pattern))  g[ly][lx] |= (Goban::Case)Patterns::_o_ << shift;
		else if (MatchPattern<Patterns::oox>(pattern))  g[ly][lx] |= (Goban::Case)Patterns::oox << shift;
		else if (MatchPattern<Patterns::oo_>(pattern))  g[ly][lx] |= (Goban::Case)Patterns::oo_ << shift;
		else if (MatchPattern<Patterns::oo>(pattern))   g[ly][lx] |= (Goban::Case)Patterns::oo << shift;
		else if (MatchPattern<Patterns::o_>(pattern))   g[ly][lx] |= (Goban::Case)Patterns::o_ << shift;
		else if (MatchPattern<Patterns::ox>(pattern))   g[ly][lx] |= (Goban::Case)Patterns::ox << shift;

        decal -= 2;
    }
}

void Goban::Putin(PION_TYPE type, unsigned int i, unsigned int j)
{
    Case & cCase = this->_map[j * 19 + i];
    cCase = (cCase & ~PIONMASK) | type;

	updatePattern<0>(*this, i, j);
	updatePattern<1>(*this, i, j);
	updatePattern<2>(*this, i, j);
	updatePattern<3>(*this, i, j);
	updatePattern<4>(*this, i, j);
	updatePattern<5>(*this, i, j);
	updatePattern<6>(*this, i, j);
	updatePattern<7>(*this, i, j);
}

void Goban::subIn(unsigned int i, unsigned int j)
{
    Case & cCase = this->_map[j * 19 + i];
	if (cCase & PIONMASK)
	{
		++this->_deletedStone[(cCase & PIONMASK) >> 1];
		cCase = (cCase & ~PIONMASK);
		updatePattern<0>(*this, i, j);
		updatePattern<1>(*this, i, j);
		updatePattern<2>(*this, i, j);
		updatePattern<3>(*this, i, j);
		updatePattern<4>(*this, i, j);
		updatePattern<5>(*this, i, j);
		updatePattern<6>(*this, i, j);
		updatePattern<7>(*this, i, j);
	}
}


