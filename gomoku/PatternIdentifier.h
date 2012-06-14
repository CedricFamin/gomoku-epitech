#ifndef PATTERNIDENTIFIER_H
#define PATTERNIDENTIFIER_H

#include <QDebug>
#include "goban.h"

namespace Patterns
{
    enum Patterns : Goban::Case
    {
        _o_ = 1,    // 0 align
        _oo,
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
    };
    static const int align_0 = _o_;
    static const int align_1 = o_;
    static const int align_2 = oo;
    static const int align_3 = ooo;
    static const int align_4 = oooo;
}

template<int direction>
class PatternIdentifier
{
public:
	template<int index>
    class Node
    {
    public:
		typedef Node<index+1> NextNode;
        Node(NextNode * sameColor, NextNode * otherColor, NextNode * empty, NextNode * wall)
            : _sameColor(sameColor), _otherColor(otherColor), _wall(wall), _empty(empty), _leaf(false)
        { }
		Node(Goban::Case pattern) : _pattern(pattern) , _leaf(true) { }
        ~Node() { }
        long long int match(Goban & g, unsigned int x, unsigned int y, Goban::Case pion)
        {
			if (this->_leaf)
				return this->_pattern;
            unsigned int lx = x + Padding<direction, index>::x;
            unsigned int ly = y + Padding<direction, index>::y;
            NextNode * branch = this->_empty;
            if (!g.InBound(lx, ly))
                branch = this->_wall;
            else
            {
                Goban::Case cpion = g[ly][lx] & Goban::PIONMASK;
                if (cpion)
                {
                    branch = this->_otherColor;
                    if (pion == 0)
                        pion = cpion;
                    if (cpion == pion)
                        branch = this->_sameColor;
                }
            }
            return (branch) ? branch->match(g, x, y, pion) : 0;
        }

    private:
        NextNode * _sameColor;
        NextNode * _otherColor;
        NextNode * _wall;
        NextNode * _empty;
		Goban::Case _pattern;
		bool _leaf;
    };

	template<>
	class Node<6> 
	{
	public:
		long long int match(Goban & g, unsigned int x, unsigned int y, Goban::Case pion)
		{
			return 0;
		}
	};

    PatternIdentifier()
    {
        _root = new Node<1>(
                    new Node<2>(
                        new Node<3>(
                            new Node<4>(
                                new Node<5>(Patterns::oooo),
                                new Node<5>(Patterns::ooox),
                                new Node<5>(Patterns::ooo_),
                                new Node<5>(Patterns::ooo)),
                            new Node<4>(Patterns::oox),
                            new Node<4>(Patterns::oo_),
                            new Node<4>(Patterns::oo)),
                        new Node<3>(Patterns::ox),
                        new Node<3>(
                            new Node<4>(
                                new Node<5>(Patterns::o_),
                                new Node<5>(Patterns::o_),
                                new Node<5>(Patterns::o_o_),
                                new Node<5>(Patterns::o_)),
                            new Node<4>(Patterns::o_),
                            new Node<4>(Patterns::o_),
                            new Node<4>(Patterns::o_)),
                        0),
                    0,
                    new Node<2>(
                        new Node<3>(
                            new Node<4>(
                                new Node<5>(Patterns::_ooo),
                                new Node<5>(Patterns::_oox),
                                new Node<5>(Patterns::_oo_),
                                new Node<5>(Patterns::_oo)),
                            0,
                            new Node<4>(Patterns::_o_),
                            0),
                        0,
                        0,
                        0),
                    0);
    }

    void match(Goban & g, int x, int y)
    {
		if (!g.InBound(x, y))
			return ;
        Goban::Case pattern = this->_root->match(g, x, y, 0);
        g[y][x] &= ~(Goban::PATTERNMASK << Goban::HEADERSIZE << (Goban::PATTERNSIZE * direction));
        g[y][x] |= pattern << Goban::HEADERSIZE << (Goban::PATTERNSIZE * direction);
    }

private:
    Node<1> * _root;
};

#endif // PATTERNIDENTIFIER_H
