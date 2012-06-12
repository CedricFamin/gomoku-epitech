#include "Goban.h"

#include <iterator>
#include "GobanIterator.h"
#include "PatternIdentifier.h"

#define MIN(x, y) ((x < y) ? x : y)

PatternIdentifier<0> pi0 = PatternIdentifier<0>();
PatternIdentifier<1> pi1 = PatternIdentifier<1>();
PatternIdentifier<2> pi2 = PatternIdentifier<2>();
PatternIdentifier<3> pi3 = PatternIdentifier<3>();
PatternIdentifier<4> pi4 = PatternIdentifier<4>();
PatternIdentifier<5> pi5 = PatternIdentifier<5>();
PatternIdentifier<6> pi6 = PatternIdentifier<6>();
PatternIdentifier<7> pi7 = PatternIdentifier<7>();

Goban::Goban(unsigned int width, unsigned int height) : _width(width), _height(height)
{
    _map = new Case*[height];
    for (unsigned int i = 0; i < height; i++)
    {
        _map[i] = new Case[width];
        for (unsigned int j = 0; j < width; j++)
        {
            _map[i][j] = 0;
        }
    }
}

Goban::Goban(Goban const & g) : _width(g._width), _height(g._height)
{
    _map = new Case*[_height];
    for (unsigned int i = 0; i < _height; i++)
    {
        _map[i] = new Case[_width];
        for (unsigned int j = 0; j < _width; j++)
        {
            _map[i][j] = g._map[i][j];
        }
    }
}

Goban::~Goban(void)
{
    for (unsigned int i = 0; i < this->_height; i++)
        delete this->_map[i];
    delete this->_map;
}

Goban::Case ** Goban::GetMap()
{
    return this->_map;
}

void Goban::update_pattern(unsigned int i, unsigned int j, int dir)
{
    int moves[8][2] = {
        { 0,-1}, { 1, -1}, { 1, 0}, { 1, 1},
        {0, 1}, {-1, 1}, { -1,0}, { -1,-1}
    };
    bool beforeColor = false;
    unsigned int color = 0, lx, ly;
    int bitDecal = 8 + 7 * dir;

    if (this->InBound(i, j) == false)
        return ;
    this->_map[j][i] &= ~(PATTERNMASK << bitDecal);
    this->_map[j][i] |= (unsigned long long int)0x1 << (bitDecal + 2);
    for (int dist = 1; dist <= 4; ++dist)
    {
        lx = i + moves[dir][0] * dist;
        ly = j + moves[dir][1] * dist;
        if (this->InBound(lx, ly))
        {
            if (color && this->_map[ly][lx] & PIONMASK && color != (this->_map[ly][lx] & PIONMASK))
            {
                if (beforeColor)
                    this->_map[j][i] &= ~((unsigned long long int)0x1 << (bitDecal + 2));
                break;
            }
            if ((this->_map[ly][lx] & PIONMASK))
            {
                beforeColor = true;
                color = this->_map[ly][lx] & PIONMASK;
                this->_map[j][i] |= (((this->_map[ly][lx] & PIONMASK)) << bitDecal);
                this->_map[j][i] |= ((unsigned long long int)0x1) << (bitDecal + 2 + dist);
            }
            else
                beforeColor = false;
        }
        else
        {
            this->_map[j][i] |= (unsigned long long int)0x1 << (bitDecal + 2);
            break;
        }
    }
}

void Goban::Putin(PION_TYPE type, unsigned int i, unsigned int j)
{
    static int const direction[8][2] = {
        { 0,-1}, { 1, -1}, { 1, 0}, { 1, 1},
        { 0, 1}, {-1,  1}, {-1, 0}, {-1,-1}
    };
    Case & cCase = this->_map[j][i];
    cCase = (cCase & ~PIONMASK) | type;
    for (int dist = 1; dist <= 4; ++dist)
    {
        pi4.match(*this, i + direction[0][0] * dist, j + direction[0][1] * dist);
        pi5.match(*this, i + direction[1][0] * dist, j + direction[1][1] * dist);
        pi6.match(*this, i + direction[2][0] * dist, j + direction[2][1] * dist);
        pi7.match(*this, i + direction[3][0] * dist, j + direction[3][1] * dist);
        pi0.match(*this, i + direction[4][0] * dist, j + direction[4][1] * dist);
        pi1.match(*this, i + direction[5][0] * dist, j + direction[5][1] * dist);
        pi2.match(*this, i + direction[6][0] * dist, j + direction[6][1] * dist);
        pi3.match(*this, i + direction[7][0] * dist, j + direction[7][1] * dist);
    }
}

void Goban::subIn(unsigned int i, unsigned int j)
{
    int direction[8][2] = {
        { 0,-1}, { 1,-1}, { 1, 0}, { 1, 1},
        { 0, 1}, {-1, 1}, {-1, 0}, {-1,-1}
    };
    Case & cCase = this->_map[j][i];

    cCase = (cCase & ~PIONMASK);
    for (int dist = 1; dist <= 4; ++dist)
    {
        pi4.match(*this, i + direction[0][0] * dist, j + direction[0][1] * dist);
        pi5.match(*this, i + direction[1][0] * dist, j + direction[1][1] * dist);
        pi6.match(*this, i + direction[2][0] * dist, j + direction[2][1] * dist);
        pi7.match(*this, i + direction[3][0] * dist, j + direction[3][1] * dist);
        pi0.match(*this, i + direction[4][0] * dist, j + direction[4][1] * dist);
        pi1.match(*this, i + direction[5][0] * dist, j + direction[5][1] * dist);
        pi2.match(*this, i + direction[6][0] * dist, j + direction[6][1] * dist);
        pi3.match(*this, i + direction[7][0] * dist, j + direction[7][1] * dist);
    }
}

unsigned int Goban::getWidth() const
{
    return this->_width;
}

unsigned int Goban::getHeight() const
{
    return this->_height;
}

bool Goban::InBound(unsigned int x, unsigned int y) const
{
    return x < this->_width && y < this->_height;
}
