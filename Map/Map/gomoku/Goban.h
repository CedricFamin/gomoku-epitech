#ifndef _GOBAN_H_
# define _GOBAN_H_

#include <stdio.h>

#include <functional>

class Goban
{
public:
    typedef long long int Case;
    static const unsigned long long int PIONMASK = 0x3;
    static const unsigned long long int PATTERNMASK = 0x7F;
    static const unsigned int HEADERSIZE = 8;
    static const unsigned int PATTERNSIZE = 7;
    static const unsigned int COLORSIZE = 2;
    static const unsigned int SAFESIZE = 1;
    static const unsigned int CONTENTSIZE = 4;
    static const unsigned long long int SUITED1 = 0x1;
    static const unsigned long long int SUITED2 = 0x3;
    static const unsigned long long int SUITED3 = 0x7;
    static const unsigned long long int SUITED4 = 0xF;
    static const unsigned long long int PA11000 = (0x1 | (Goban::SUITED1 << Goban::SAFESIZE) << Goban::COLORSIZE);
    static const unsigned long long int PA10110 = (0x1 | (Goban::SUITED2 << (Goban::SAFESIZE + 1) ) << Goban::COLORSIZE);
    static const unsigned long long int PA11100 = (0x1 | (Goban::SUITED2 << Goban::SAFESIZE) << Goban::COLORSIZE);

    enum PION_TYPE
    {
        EMPTY,
        BLACK = 0x1,
        RED = 0x3
    };

    Goban(unsigned int = 19, unsigned int = 19);
    Goban(Goban const &);
    ~Goban(void);

    Case ** GetMap();
    void Putin(PION_TYPE, unsigned int, unsigned int);
    void subIn(unsigned int, unsigned int);

    unsigned int getWidth() const;
    unsigned int getHeight() const;

    bool InBound(unsigned int, unsigned int) const;

protected:

private:
    Case **_map;
    int update_serie(unsigned int i, unsigned int j, int decal_x, int decal_y, int dir, int depth);
    void update_pattern(unsigned int, unsigned int, int);
    unsigned int _width;
    unsigned int _height;
};

template<int nbBits>
struct PatternMask
{
    static const unsigned long long int value = (1 << (nbBits - 1)) | PatternMask<nbBits - 1>::value;
};
template<>
struct PatternMask<0>
{
    static const unsigned long long int value = 0;
};

template<bool safe, int size, unsigned long long int content>
struct Pattern
{
    static const unsigned long long int value = (safe | (content << 1)) << Goban::COLORSIZE;
    static const unsigned long long int mask = PatternMask<size + safe + Goban::COLORSIZE>::value;
};

#endif
