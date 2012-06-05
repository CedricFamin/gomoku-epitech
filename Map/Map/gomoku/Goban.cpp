#include "Goban.h"

#include <iterator>
#include "GobanIterator.h"

#define MIN(x, y) ((x < y) ? x : y)

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
    int direction[8][2] = {
        { 0,-1}, { 1, -1}, { 1, 0}, { 1, 1},
        {0, 1}, {-1, 1}, { -1,0}, { -1,-1}
    };
    Case & cCase = this->_map[j][i];
    cCase = (cCase & ~PIONMASK) | type;
    for (int dir = 0; dir < 8; ++dir)
    {
        update_pattern(i - direction[dir][0] * 1, j - direction[dir][1] * 1, dir);
        update_pattern(i - direction[dir][0] * 2, j - direction[dir][1] * 2, dir);
        update_pattern(i - direction[dir][0] * 3, j - direction[dir][1] * 3, dir);
        update_pattern(i - direction[dir][0] * 4, j - direction[dir][1] * 4, dir);
    }
}

void Goban::subIn(unsigned int i, unsigned int j)
{
    int direction[8][2] = {
        { 0,-1}, { 1, -1}, { 1, 0}, { 1, 1},
        {0, 1}, {-1, 1}, { -1,0}, { -1,-1}
    };
    Case & cCase = this->_map[j][i];

    cCase = (cCase & ~PIONMASK);
    for (int dir = 0; dir < 8; ++dir)
    {
        update_pattern(i - direction[dir][0] * 1, j - direction[dir][1] * 1, dir);
        update_pattern(i - direction[dir][0] * 2, j - direction[dir][1] * 2, dir);
        update_pattern(i - direction[dir][0] * 3, j - direction[dir][1] * 3, dir);
        update_pattern(i - direction[dir][0] * 4, j - direction[dir][1] * 4, dir);
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
