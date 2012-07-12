#include <PatternIdentifier.h>

namespace Patterns
{
	const PatternInfos patterns[PATTERNNUMBER] = 
	{
		PatternInfos(),
		PatternInfos(o___, 1, 3, 1, 1, 4),
		PatternInfos(_o__, 0, 3, 1, 2, 4),
		PatternInfos(__o_, 0, 3, 1, 3, 4),
		PatternInfos(___o, 0, 3, 1, 4, 4),
		PatternInfos(ox,   1, 0, 0, 1, 1),
		PatternInfos(o_x,  1, 1, 1, 1, 2),
		PatternInfos(_ox,  0, 1, 0, 2, 2),
		PatternInfos(o__x, 1, 2, 1, 1, 3),
		PatternInfos(_o_x, 0, 2, 1, 2, 3),
		PatternInfos(__ox, 0, 2, 0, 3, 3),
		PatternInfos(oo__, 2, 2, 1, 1, 4),
		PatternInfos(_oo_, 0, 2, 1, 2, 4),
		PatternInfos(__oo, 0, 2, 0, 3, 4),
		PatternInfos(o_o_, 1, 2, 1, 1, 4),
		PatternInfos(_o_o, 0, 2, 0, 2, 4),
		PatternInfos(o__o, 1, 2, 0, 1, 4),
		PatternInfos(oox,  2, 0, 0, 1, 2),
		PatternInfos(oo_x, 2, 2, 1, 1, 3),
		PatternInfos(_oox, 1, 1, 0, 2, 3),
		PatternInfos(o_ox, 1, 1, 1, 1, 3),
		PatternInfos(ooo_, 3, 1, 1, 1, 4),
		PatternInfos(_ooo, 0, 1, 0, 2, 4),
		PatternInfos(oo_o, 2, 1, 1, 1, 4),
		PatternInfos(o_oo, 1, 1, 1, 1, 4),
		PatternInfos(oooo, 4, 0, 0, 1, 4),
		PatternInfos(ooox, 3, 0, 0, 1, 3)
	};
}