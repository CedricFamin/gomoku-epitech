#ifndef _GOBAN_H_
# define _GOBAN_H_

#include <stdio.h>
#include <assert.h>

#ifndef WIN32
#include <tr1/functional>
#else
#include <functional>
#endif

#include <list>
#include <stack>

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
    static const long long int PATTERNMASK = 0x1F;
    static const unsigned int HEADERSIZE = 2;
    static const unsigned int PATTERNSIZE = 5;
    static const unsigned int COLORSIZE = 2;
    static const unsigned int CONTENTSIZE = 5;
	static const unsigned int INFLUENCESIZE = 8;
	static const unsigned int BLACKINFLUENCEINDEX = HEADERSIZE + PATTERNSIZE * 8;
	static const unsigned int WHITEINFLUENCEINDEX = HEADERSIZE + PATTERNSIZE * 8 + INFLUENCESIZE;
	static const long long int INFLUENCEMASK = 0xFF;
    enum PION_TYPE
    {
        EMPTY,
        BLACK = 0x1,
        RED = 0x2
    };

	inline static PION_TYPE Other(PION_TYPE pion) {
		assert(pion == BLACK || pion == RED);
		return (pion == BLACK) ? RED : BLACK; 
	}

	inline static int GetInfluenceIndex(PION_TYPE pion) {
		assert(pion == BLACK || pion == RED);
		return BLACKINFLUENCEINDEX + ((pion==BLACK)?0:8);
	}

	inline static int GetInfluence(Case c, PION_TYPE p) {
		return (c >> GetInfluenceIndex(p)) & INFLUENCEMASK;
	}

	inline static Case GetPattern(Case c, int d) {
		return (c >> (HEADERSIZE + PATTERNSIZE * d)) & PATTERNMASK;
	}

	struct Turn
	{
		inline Turn(PION_TYPE p, unsigned int x, unsigned int y) : pion(p), x(x), y(y) {}
        Goban::PION_TYPE pion;
        unsigned int x;
		unsigned int y;
		bool validate;
        //std::list<std::pair<unsigned int, unsigned int> > captures;
	};

	struct Align
	{
		unsigned int x;
		unsigned int y;
		unsigned int dir;
		unsigned int size;
		PION_TYPE pion;
	};

    Goban();
    Goban(Goban const &);
    ~Goban(void);

    void Putin(PION_TYPE, unsigned int, unsigned int);
    void subIn(unsigned int, unsigned int, bool = true);
	void clear();

	inline bool gameFinished(void)								const { return this->_gameFinished; }
	inline unsigned int getWidth()								const { return this->_width; }
	inline unsigned int getHeight()								const { return this->_height; }
	inline Goban::PION_TYPE getWinner()							const { return this->_winner; }
    inline bool InBound(unsigned int x, unsigned int y)	const { return x < this->_width && y < this->_height; }
    inline int deletedStone(Goban::PION_TYPE pion)		const { return this->_deletedStone[pion>>1]; }
	inline int& setDeletedStone(Goban::PION_TYPE pion)				  { return this->_deletedStone[pion>>1]; }
	inline void setGameFinished(bool value)							  { this->_gameFinished = value; }
	inline void setWinner(Goban::PION_TYPE pion)					  { this->_winner = pion; }
	inline std::list<Turn> & Turns()								  { return this->_turnList; }

	std::list<Align> alignments;
	std::stack<std::pair<unsigned int, unsigned int>> toDelete;

	Case * operator[](unsigned int);

protected:

private:
    Case _map[19 * 19];
    unsigned int _width;
    unsigned int _height;
	bool _gameFinished;
	int _deletedStone[2];
	Goban::PION_TYPE _winner;
	std::list<Turn> _turnList;
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
