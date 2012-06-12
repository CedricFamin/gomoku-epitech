#ifndef PATTERNIDENTIFIER_H
#define PATTERNIDENTIFIER_H

#include <QDebug>
#include "goban.h"

namespace Patterns
{
    enum Patterns
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
    static const int moves[8][2];
    static Goban::Case pion;

    class INode
    {
    public:
        virtual unsigned long long int match(Goban & g, unsigned int, unsigned int) = 0;
    };

    template<int index>
    class Node : public INode
    {
    public:
        Node(INode * sameColor, INode * otherColor, INode * empty, INode * wall)
            : _sameColor(sameColor), _otherColor(otherColor), _wall(wall), _empty(empty)
        { }
        virtual ~Node() { }
        virtual unsigned long long int match(Goban & g, unsigned int x, unsigned int y)
        {
            unsigned int lx = x + moves[direction][0] * index;
            unsigned int ly = y + moves[direction][1] * index;
            INode * branch = this->_empty;
            if (!g.InBound(lx, ly))
                branch = this->_wall;
            else
            {
                Goban::Case pion = g.GetMap()[ly][lx] & Goban::PIONMASK;
                if (pion)
                {
                    if (!PatternIdentifier::pion)
                        PatternIdentifier::pion = pion;
                    if (pion == PatternIdentifier::pion)
                        branch = this->_sameColor;
                    else if (pion)
                        branch = this->_otherColor;
                }
            }
            if (branch)
                return branch->match(g, x, y);
            return 0;

        }

    private:
        INode * _sameColor;
        INode * _otherColor;
        INode * _wall;
        INode * _empty;
    };

    class Leaf : public INode
    {
    public:
        Leaf(Goban::Case pattern) : _pattern(pattern) {}
        virtual ~Leaf() {}
        virtual unsigned long long int match(Goban &, unsigned int, unsigned int)
        {
            return this->_pattern;
        }
    private:
        Goban::Case _pattern;

    };

    PatternIdentifier()
    {
        _root = new Node<1>(
                    new Node<2>(
                        new Node<3>(
                            new Node<4>(
                                new Leaf(Patterns::oooo),
                                new Leaf(Patterns::ooox),
                                new Leaf(Patterns::ooo_),
                                new Leaf(Patterns::ooo)),
                            new Leaf(Patterns::oox),
                            new Leaf(Patterns::oo_),
                            new Leaf(Patterns::oo)),
                        new Leaf(Patterns::ox),
                        new Node<3>(
                            new Node<4>(
                                new Leaf(Patterns::o_),
                                new Leaf(Patterns::o_),
                                new Leaf(Patterns::o_o_),
                                new Leaf(Patterns::o_)),
                            new Leaf(Patterns::o_),
                            new Leaf(Patterns::o_),
                            new Leaf(Patterns::o_)),
                        0),
                    0,
                    new Node<2>(
                        new Node<3>(
                            new Node<4>(
                                new Leaf(Patterns::_ooo),
                                new Leaf(Patterns::_oox),
                                new Leaf(Patterns::_oo_),
                                new Leaf(Patterns::_oo)),
                            0,
                            new Leaf(Patterns::_o_),
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
        pion = 0;
        Goban::Case pattern = 0;
        pattern = this->_root->match(g, x, y);
        g.GetMap()[y][x] &= ~(Goban::PATTERNMASK << Goban::HEADERSIZE << (Goban::PATTERNSIZE * direction));
        g.GetMap()[y][x] |= pattern << Goban::HEADERSIZE << (Goban::PATTERNSIZE * direction);

    }

private:
    INode * _root;
};

template<int direction>
int const PatternIdentifier<direction>::moves[8][2]  = {{ 0,-1}, { 1, -1}, { 1, 0}, { 1, 1},
                                                        { 0, 1}, {-1,  1}, {-1, 0}, {-1,-1}};
template<int direction>
Goban::Case PatternIdentifier<direction>::pion = 0;

#endif // PATTERNIDENTIFIER_H
