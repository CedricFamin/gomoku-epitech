#ifndef GOBANITERATOR_H_
# define GOBANITERATOR_H_

#include <iterator>
#include "Goban.h"

class GobanIterator : public std::iterator<std::bidirectional_iterator_tag, Goban::Case>
{
public:
	static const int direction[8][2];
	GobanIterator(Goban &, unsigned int x, unsigned int y);
	GobanIterator(GobanIterator const & it);
	GobanIterator& operator++();
	GobanIterator& operator--();
	bool operator==(GobanIterator const & it) const;
	bool operator!=(GobanIterator const & it) const;
	Goban::Case& operator[](unsigned int);
	Goban::Case& operator*();
	Goban::Case* operator->();

	bool isNull() const;
	int dir() const;
	GobanIterator makeEndIterator() const;
private:
	unsigned int _baseX, _baseY, _maxX, _maxY, _position;
	Goban _map;
};

#endif