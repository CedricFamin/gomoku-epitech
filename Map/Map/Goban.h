#ifndef _GOBAN_H_
# define _GOBAN_H_

#include <functional>

#define TURN_AROUND(map, x, y, maxX, maxY)											\
	int TURN_AROUND_DECAL[8][2] =													\
	{																				\
		{ 0,-1}, { 1, 0}, { 0, 1}, { 0, 1},											\
		{-1, 0}, {-1, 0}, { 0,-1}, { 0,-1}											\
	};																				\
	unsigned int TURN_X = x + TURN_AROUND_DECAL[0][0];								\
	unsigned int TURN_Y = y + TURN_AROUND_DECAL[0][1];								\
	auto currentElem = (TURN_X < maxX && TURN_Y < maxY) ? &map[TURN_Y][TURN_X]:0;	\
	for (int TURN_INDEX = 0; TURN_INDEX < 8;										\
		TURN_INDEX++,																\
		TURN_X += TURN_AROUND_DECAL[TURN_INDEX][0],									\
		TURN_Y += TURN_AROUND_DECAL[TURN_INDEX][1],									\
		currentElem = ((TURN_X < maxX && TURN_Y < maxY) ? &map[TURN_Y][TURN_X]:0))

#define GETPIONTYPE(x) ((Goban::PION_TYPE)(x.color))
#define EXTRACTBIT(x, b) (x & 1 << b)
/* PATTERNS
** pair safe	 . o o .	| o o .		| x o o x | o o o .
** pair non safe x o o .
** trois		 o o o		| o o . o
** trois libre	 . o o o .	| . o o . o .
** quatre		 o o o o
*/

class Goban
{
public:
	static const char TOP		= 0x1;
	static const char TOPRIGHT	= 0x2;
	static const char RIGHT		= 0x3;
	static const char BOTRIGHT	= 0x4;
	static const char BOT		= 0x5;
	static const char BOTLEFT	= 0x6;
	static const char LEFT		= 0x7;
	static const char TOPLEFT	= 0x8;

	union CharDirBitField
	{
		struct
		{
			unsigned char top:1;
			unsigned char topRight:1;
			unsigned char right:1;
			unsigned char botRight:1;
			unsigned char bot:1;
			unsigned char botLeft:1;
			unsigned char left:1;
			unsigned char topLeft:1;
		};
		unsigned char rawData;
	};
	union Case
	{
		struct 
		{
			CharDirBitField pair;
			CharDirBitField pair_safe;
			unsigned char color:2;
			unsigned char padding:6;
			CharDirBitField free_case;
		};
		unsigned int rawData;
	};
	enum PION_TYPE
	{
		EMPTY,
		BLACK = 0x1,
		RED = 0x3
	};

	Goban(unsigned int = 19, unsigned int = 19);
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
	static void alloc_map(char **&map, unsigned int, unsigned int);
	void _update_pair(Case &, unsigned int, unsigned int);
	void _update_safe(Case &, unsigned int, unsigned int);
	void _update_free(Case &, unsigned int, unsigned int);

	unsigned int _width;
	unsigned int _height;
};

#endif