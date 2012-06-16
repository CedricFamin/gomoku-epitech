#ifndef _GOBAN_H_
# define _GOBAN_H_

#include <stdio.h>

#include <functional>

template<int direction> struct Moves { };
template<> struct Moves<0> { enum Values {x = 0, y =-1 };};
template<> struct Moves<1> { enum Values {x = 1, y =-1 };};
template<> struct Moves<2> { enum Values {x = 1, y = 0 };};
template<> struct Moves<3> { enum Values {x = 1, y = 1 };};
template<> struct Moves<4> { enum Values {x = 0, y = 1 };};
template<> struct Moves<5> { enum Values {x =-1, y = 1 };};
template<> struct Moves<6> { enum Values {x =-1, y = 0 };};
template<> struct Moves<7> { enum Values {x =-1, y =-1 };};

template<int direction, int dist>
struct Padding
{
	enum Values
	{
		x = Moves<direction>::x * dist,
		y = Moves<direction>::y * dist
	};
};


class Goban
{
public:
    typedef long long int Case;
    typedef std::pair<unsigned int, unsigned int> Move;
    static const long long int PIONMASK = 0x3;
    static const long long int PATTERNMASK = 0xF;
    static const unsigned int HEADERSIZE = 2;
    static const unsigned int PATTERNSIZE = 4;
    static const unsigned int COLORSIZE = 2;
    static const unsigned int CONTENTSIZE = 4;
	static const unsigned int SCOREINDEX = HEADERSIZE + PATTERNSIZE * 8;
	static const long long int SCOREMASK = 0xFFFF;
    enum PION_TYPE
    {
        EMPTY,
        BLACK = 0x1,
        RED = 0x2
    };

    Goban();
    Goban(Goban const &);
    ~Goban(void);

    void Putin(PION_TYPE, unsigned int, unsigned int);
    void subIn(unsigned int, unsigned int);

	inline bool gameFinished(void)								const { return this->_gameFinished; }
	inline unsigned int getWidth()								const { return this->_width; }
	inline unsigned int getHeight()								const { return this->_height; }
	inline Goban::PION_TYPE getWinner()							const { return this->_winner; }
	inline bool Goban::InBound(unsigned int x, unsigned int y)	const { return x < this->_width && y < this->_height; }
	inline int deletedStone(Goban::PION_TYPE pion)					  { return this->_deletedStone[pion>>1]; }
	inline void setGameFinished(bool value)							  { this->_gameFinished = value; }
	inline void setWinner(Goban::PION_TYPE pion)					  { this->_winner = pion; }

	Case * operator[](unsigned int);

protected:

private:
    Case _map[19 * 19];
    unsigned int _width;
    unsigned int _height;
	bool _gameFinished;
	int _deletedStone[2];
	Goban::PION_TYPE _winner;
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
