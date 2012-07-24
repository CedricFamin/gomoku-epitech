#include "Goban.h"

#include <iterator>
#include <map>
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
	nbTurn = 0;
}

Goban::Goban(Goban const & g) : _width(g._width), _height(g._height)
{
	_winner = g._winner;
	_gameFinished = g._gameFinished;
	_deletedStone[0] = g._deletedStone[0];
	_deletedStone[1] = g._deletedStone[1];
	memcpy(_map,g._map, sizeof(*_map) * 19 * 19);
	nbTurn = g.nbTurn;
}

Goban::~Goban(void)
{
}

Goban::Case * Goban::operator[](unsigned int i)
{
	return this->_map + i * 19;
}

Goban::Case const * Goban::operator[](unsigned int i) const
{
	return this->_map + i * 19;
}

inline void SetInfluence(Goban & g, unsigned int x, unsigned int y, Goban::Case pattern, int dist, Goban::PION_TYPE pion)
{
	Goban::PION_TYPE color = Goban::EMPTY;
	int i, shift = 0;
	bool add=true;

	for (i = 0; i < dist; ++i) {
		if ((pattern & 0x3) == Goban::BLACK || (pattern & 0x3) == Goban::RED) {
			if (color == Goban::EMPTY)
				color = static_cast<Goban::PION_TYPE>(pattern & 0x3);
			else if (color != (pattern & 0x3))  {
				pattern >>= 2;
				break;
			}
		}
		pattern >>= 2;
	}
	if (++i < dist) return ;
	if (color == Goban::EMPTY || color == pion) {
		if ((pattern & 0x3)!= pion) add = false; 
		shift = (Goban::BLACKINFLUENCEINDEX + ((pion == Goban::RED)?8:0));
		Goban::Case current = g[y][x];
		Goban::Case lastScore = ((current >> shift) & Goban::INFLUENCEMASK) + (add?1:-1);
        if (lastScore >= 0) g[y][x] = (current & ~(Goban::INFLUENCEMASK << shift)) | lastScore << shift;
		if (color == Goban::EMPTY) color = pion;
		else  return;
	}
	for (; i < 4; ++i)
	{
		pattern >>= 2;
		if ((pattern & 0x3) == color)
			break;
		if ((pattern & 0x3) == Goban::Other(color))
		{
			shift = Goban::BLACKINFLUENCEINDEX + ((color == Goban::BLACK)?8:0);
			Goban::Case lastScore = ((g[y][x] >> shift) & Goban::INFLUENCEMASK) - (add?1:-1);
			if (lastScore >= 0)
			{
                g[y][x] = (g[y][x] & ~(Goban::INFLUENCEMASK << shift)) | lastScore << shift;
			}
		}
	}
}

template<int direction>
inline void updatePattern(Goban & g, unsigned int x, unsigned int y, Goban::PION_TYPE)
{
	static std::vector<Goban::Case> PatternMap, Pattern0xF, Pattern0x3F;
	if (PatternMap.size() == 0)
	{
		Pattern0xF.resize(0xF);
		Pattern0xF[0x9 ] = Patterns::ox;
		Pattern0xF[0x6 ] = Patterns::ox;

		Pattern0x3F.resize(0x3F);
		Pattern0x3F[0x21] = Patterns::o_x;
		Pattern0x3F[0x12] = Patterns::o_x;
		Pattern0x3F[0x24] = Patterns::_ox;
		Pattern0x3F[0x18] = Patterns::_ox;
		Pattern0x3F[0x25] = Patterns::oox;
		Pattern0x3F[0x1A] = Patterns::oox;

		PatternMap.resize(0xFF);
		PatternMap[0x1 ] = Patterns::o___;
		PatternMap[0x2 ] = Patterns::o___;
		PatternMap[0x4 ] = Patterns::_o__;
		PatternMap[0x8 ] = Patterns::_o__;
		PatternMap[0x10] = Patterns::__o_;
		PatternMap[0x20] = Patterns::__o_;
		PatternMap[0x40] = Patterns::___o;
		PatternMap[0x80] = Patterns::___o;
		PatternMap[0x81] = Patterns::o__x;
		PatternMap[0x42] = Patterns::o__x;
		PatternMap[0x84] = Patterns::_o_x;
		PatternMap[0x48] = Patterns::_o_x;
		PatternMap[0x90] = Patterns::__ox;
		PatternMap[0x60] = Patterns::__ox;
		PatternMap[0x5 ] = Patterns::oo__;
		PatternMap[0xA ] = Patterns::oo__;
		PatternMap[0x14] = Patterns::_oo_;
		PatternMap[0x28] = Patterns::_oo_;
		PatternMap[0x50] = Patterns::__oo;
		PatternMap[0xA0] = Patterns::__oo;
		PatternMap[0x11] = Patterns::o_o_;
		PatternMap[0x22] = Patterns::o_o_;
		PatternMap[0x44] = Patterns::_o_o;
		PatternMap[0x88] = Patterns::_o_o;
		PatternMap[0x41] = Patterns::o__o;
		PatternMap[0x82] = Patterns::o__o;
		PatternMap[0x85] = Patterns::oo_x;
		PatternMap[0x4A] = Patterns::oo_x;
		PatternMap[0x94] = Patterns::_oox;
		PatternMap[0x68] = Patterns::_oox;
		PatternMap[0x91] = Patterns::o_ox;
		PatternMap[0x62] = Patterns::o_ox;
		PatternMap[0x15] = Patterns::ooo_;
		PatternMap[0x2A] = Patterns::ooo_;
		PatternMap[0x54] = Patterns::_ooo;
		PatternMap[0xA8] = Patterns::_ooo;
		PatternMap[0x45] = Patterns::oo_o;
		PatternMap[0x8A] = Patterns::oo_o;
		PatternMap[0x51] = Patterns::o_oo;
		PatternMap[0xA2] = Patterns::o_oo;
		PatternMap[0x55] = Patterns::oooo;
		PatternMap[0xAA] = Patterns::oooo;
		PatternMap[0x95] = Patterns::ooox;
		PatternMap[0x6A] = Patterns::ooox;
	}

    unsigned int shift = (direction + 4) * Goban::PATTERNSIZE + Goban::HEADERSIZE;
	unsigned int rshift = direction * Goban::PATTERNSIZE + Goban::HEADERSIZE;
    Goban::Case cases = 0, rcase = 0;
    unsigned int decal = 6;
    Goban::Case pattern;
    unsigned int lx  = x - Padding<direction,3>::x, ly  = y - Padding<direction,3>::y;
	unsigned int rlx = x + Padding<direction,3>::x, rly = y + Padding<direction,3>::y;
	Goban::Case patternIndex;

    for (int i = 0; i < 7; ++i)
	{
		cases <<= 2;
		rcase <<= 2;
		cases |= g.InBound(lx, ly)   ? g[ly][lx] & Goban::PIONMASK   : 0x0;
		rcase |= g.InBound(rlx, rly) ? g[rly][rlx] & Goban::PIONMASK : 0x0;
        lx += Padding<direction,1>::x;
        ly += Padding<direction,1>::y;
		rlx -= Padding<direction,1>::x;
        rly -= Padding<direction,1>::y;
	}
    lx = x;  ly = y;
	rlx = x; rly = y;
    for (int i = 0; i < 4; ++i)
	{
        lx += Padding<direction,1>::x;
        ly += Padding<direction,1>::y;
		rlx -= Padding<direction,1>::x;
		rly -= Padding<direction,1>::y;
		if (g.InBound(rlx, rly))
        {
			pattern = rcase >> decal;
			g[rly][rlx] &= ~(Goban::PATTERNMASK << rshift);
			if (!(patternIndex = Pattern0xF[pattern&0xF]))
				if (!(patternIndex = Pattern0x3F[pattern&0x3F]))
					patternIndex = PatternMap[pattern&0xFF];
			g[rly][rlx] |= patternIndex << rshift;
			//SetInfluence(g,rlx,rly,pattern,i,pion);
		}
        if (g.InBound(lx, ly))
        {
			pattern = cases >> decal;
			g[ly][lx] &= ~(Goban::PATTERNMASK << shift);
			if (!(patternIndex = Pattern0xF[pattern&0xF]))
				if (!(patternIndex = Pattern0x3F[pattern&0x3F]))
					patternIndex = PatternMap[pattern&0xFF];
			g[ly][lx] |= patternIndex  << shift;
			//SetInfluence(g,lx,ly,pattern,i,pion);
		}
		
        decal -= 2;
    }
}

void Goban::Putin(PION_TYPE type, unsigned int i, unsigned int j)
{
    Case & cCase = this->_map[j * 19 + i];
    cCase = (cCase & ~PIONMASK) | type;

	updatePattern<0>(*this, i, j, type);
	updatePattern<1>(*this, i, j, type);
	updatePattern<2>(*this, i, j, type);
	updatePattern<3>(*this, i, j, type);
}

void Goban::subIn(unsigned int i, unsigned int j, bool count)
{
    Case & cCase = this->_map[j * 19 + i];
	if (cCase & PIONMASK)
	{
		if (count)
			++this->_deletedStone[(cCase & PIONMASK) >> 1];
		PION_TYPE pion = static_cast<Goban::PION_TYPE>(cCase & PIONMASK);
		cCase = (cCase & ~PIONMASK);
		updatePattern<0>(*this, i, j, pion);
		updatePattern<1>(*this, i, j, pion);
		updatePattern<2>(*this, i, j, pion);
		updatePattern<3>(*this, i, j, pion);
	}
}

void Goban::clear()
{
	this->_deletedStone[0] = 0;
	this->_deletedStone[1] = 0;
	this->_winner = EMPTY;
	this->_gameFinished = false;
	nbTurn = 0;
	memset(this->_map, 0, this->_height * this->_width * sizeof(*this->_map));
}
