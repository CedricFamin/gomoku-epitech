#ifndef PATTERNIDENTIFIER_H
#define PATTERNIDENTIFIER_H

#include <QDebug>
#include "Goban.h"

namespace Patterns
{
	enum Patterns
	{
		____ = 0,
		o___ = 1,
		_o__,
		__o_,
		___o,
		ox,
		o_x,
		_ox,
		o__x,
		_o_x,
		__ox,
		oo__,
		_oo_,
		__oo,
		o_o_,
		_o_o,
		o__o,
		oox,
		oo_x,
		_oox,
		o_ox,
		ooo_,
		_ooo,
		oo_o,
		o_oo,
		oooo,
		ooox,
		PATTERNNUMBER
	};
	template<unsigned int p> struct PatternsInfos { };
	struct PatternInfos
	{
		inline PatternInfos() : pattern(____), align(0), expand(4), caseIndex(0), free(true), size(4) {}
		inline PatternInfos(Goban::Case p, int a, int f, bool free, int caseIndex, int size) : pattern(p), align(a), expand(f), caseIndex(caseIndex), free(free), size(size) {}
		Goban::Case pattern;
		int align;
		int expand;
		int caseIndex;
		int size;
		bool free;
	};

	extern PatternInfos patterns[PATTERNNUMBER];

	inline PatternInfos GetPatternInfos(unsigned int pattern) 
	{
		if (pattern < PATTERNNUMBER)
			return patterns[pattern];
		return PatternInfos();
	}
	
	template<> struct PatternsInfos<o___>   { enum values { align = 1, expand = 3, black = 0x1 , white = 0x2 , mask = 0xFF, free = 1, caseIndex = 1, size = 4}; };
	template<> struct PatternsInfos<_o__>   { enum values { align = 0, expand = 3, black = 0x4 , white = 0x8 , mask = 0xFF, free = 1, caseIndex = 2, size = 4}; };
	template<> struct PatternsInfos<__o_>   { enum values { align = 0, expand = 3, black = 0x10, white = 0x20, mask = 0xFF, free = 1, caseIndex = 3, size = 4}; };
	template<> struct PatternsInfos<___o>   { enum values { align = 0, expand = 3, black = 0x40, white = 0x80, mask = 0xFF, free = 0, caseIndex = 4, size = 4}; };
	template<> struct PatternsInfos<ox>     { enum values { align = 1, expand = 0, black = 0x9 , white = 0x6 , mask = 0xF , free = 0, caseIndex = 1, size = 2}; };
	template<> struct PatternsInfos<o_x>    { enum values { align = 1, expand = 1, black = 0x21, white = 0x12, mask = 0x3F, free = 1, caseIndex = 1, size = 3}; };
	template<> struct PatternsInfos<_ox>    { enum values { align = 0, expand = 1, black = 0x24, white = 0x18, mask = 0x3F, free = 0, caseIndex = 2, size = 3}; };
	template<> struct PatternsInfos<o__x>   { enum values { align = 1, expand = 2, black = 0x81, white = 0x42, mask = 0xFF, free = 1, caseIndex = 1, size = 4}; };
	template<> struct PatternsInfos<_o_x>   { enum values { align = 0, expand = 2, black = 0x84, white = 0x48, mask = 0xFF, free = 1, caseIndex = 2, size = 4}; };
	template<> struct PatternsInfos<__ox>   { enum values { align = 0, expand = 2, black = 0x90, white = 0x60, mask = 0xFF, free = 0, caseIndex = 3, size = 4}; };
	template<> struct PatternsInfos<oo__>   { enum values { align = 2, expand = 2, black = 0x5 , white = 0xA , mask = 0xFF, free = 1, caseIndex = 1, size = 4}; };
	template<> struct PatternsInfos<_oo_>   { enum values { align = 0, expand = 2, black = 0x14, white = 0x28, mask = 0xFF, free = 1, caseIndex = 2, size = 4}; };
	template<> struct PatternsInfos<__oo>   { enum values { align = 0, expand = 2, black = 0x50, white = 0xA0, mask = 0xFF, free = 0, caseIndex = 3, size = 4}; };
	template<> struct PatternsInfos<o_o_>   { enum values { align = 1, expand = 2, black = 0x11, white = 0x22, mask = 0xFF, free = 1, caseIndex = 1, size = 4}; };
	template<> struct PatternsInfos<_o_o>   { enum values { align = 0, expand = 2, black = 0x44, white = 0x88, mask = 0xFF, free = 0, caseIndex = 2, size = 4}; };
	template<> struct PatternsInfos<o__o>   { enum values { align = 1, expand = 2, black = 0x41, white = 0x82, mask = 0xFF, free = 0, caseIndex = 1, size = 4}; };
	template<> struct PatternsInfos<oox>    { enum values { align = 2, expand = 0, black = 0x25, white = 0x1A, mask = 0x3F, free = 0, caseIndex = 1, size = 3}; };
	template<> struct PatternsInfos<oo_x>   { enum values { align = 2, expand = 2, black = 0x85, white = 0x4A, mask = 0xFF, free = 1, caseIndex = 1, size = 4}; };
	template<> struct PatternsInfos<_oox>   { enum values { align = 1, expand = 1, black = 0x94, white = 0x68, mask = 0xFF, free = 0, caseIndex = 2, size = 4}; };
	template<> struct PatternsInfos<o_ox>   { enum values { align = 1, expand = 1, black = 0x91, white = 0x62, mask = 0xFF, free = 1, caseIndex = 1, size = 4}; };
	template<> struct PatternsInfos<ooo_>   { enum values { align = 3, expand = 0, black = 0x15, white = 0x2A, mask = 0xFF, free = 1, caseIndex = 1, size = 4}; };
	template<> struct PatternsInfos<_ooo>   { enum values { align = 0, expand = 1, black = 0x54, white = 0xA8, mask = 0xFF, free = 0, caseIndex = 2, size = 4}; };
	template<> struct PatternsInfos<oo_o>   { enum values { align = 2, expand = 1, black = 0x45, white = 0x8A, mask = 0xFF, free = 1, caseIndex = 1, size = 4}; };
	template<> struct PatternsInfos<o_oo>   { enum values { align = 1, expand = 1, black = 0x51, white = 0xA2, mask = 0xFF, free = 1, caseIndex = 1, size = 4}; };
	template<> struct PatternsInfos<oooo>   { enum values { align = 4, expand = 0, black = 0x55, white = 0xAA, mask = 0xFF, free = 0, caseIndex = 1, size = 4}; };
	template<> struct PatternsInfos<ooox>   { enum values { align = 3, expand = 0, black = 0x95, white = 0x6A, mask = 0xFF, free = 0, caseIndex = 1, size = 4}; };

    /*enum Patterns : Goban::Case
    {
        _o_ = 1,    // 0 align
        _oo,
		_ox,
        _oo_,
        _oox,
        _ooo,
        o_,     // 1 align
        o_o_,
        ox,
        oo,     // 2 align
        oo_,
        oox,
        ooo,    // 3 align
        ooo_,
        ooox,
        oooo    // 4 align
    };*/
    /*static const int align_0 = _o_;
    static const int align_1 = o_;
    static const int align_2 = oo;
    static const int align_3 = ooo;
    static const int align_4 = oooo;
	*/
}

#endif // PATTERNIDENTIFIER_H
