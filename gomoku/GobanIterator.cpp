#include <stdexcept>
#include "GobanIterator.h"

const int GobanIterator::direction[8][2] = {
		{ 0,-1}, { 1, -1}, { 1, 0}, { 1, 1},
		{0, 1}, {-1, 1}, { -1,0}, { -1,-1}
	};

GobanIterator::GobanIterator(Goban & g, unsigned int x, unsigned int y) : _baseX(x), _baseY(y), _maxX(g.getWidth()), _maxY(g.getHeight()), _position(0), _map(g) { }
GobanIterator::GobanIterator(GobanIterator const & it) : _baseX(it._baseX), _baseY(it._baseY), _maxX(it._maxX), _maxY(it._maxY), _position(it._position), _map(it._map) {}

GobanIterator& GobanIterator::operator++() {
	if (this->_position < 8)
		this->_position++;
	else 
		this->_position = 0;
	return *this;
}
GobanIterator& GobanIterator::operator--() {
	if (this->_position > 0)
		this->_position--;
	else 
		this->_position = 8;
	return *this;
}
bool GobanIterator::operator==(GobanIterator const & it) const
{
	if (this == &it)
		return true;
	if (this->_baseX == it._baseX && this->_baseY == it._baseY && this->_maxX == it._maxX && this->_maxY == it._maxY)
	{
		if (this->_position >= 0 && this->_position < 8)
		{
			if (it._position >= 0 && it._position < 8)
				return this->_position == it._position;
		}
		else if (it._position >= 0 && it._position < 8) return false;
		else return true;
	}
	return false;
}

bool GobanIterator::operator!=(GobanIterator const & it) const
{
	return !(*this == it);
}

Goban::Case& GobanIterator::operator[](unsigned int i)
{
	unsigned int lx = this->_baseX + direction[i][0];
	unsigned int ly = this->_baseY + direction[i][1];

	if (lx >= this->_maxX || ly >= this->_maxY)
		throw std::out_of_range("Case not found");
	return this->_map[ly][lx];

}

Goban::Case& GobanIterator::operator*()
{
	unsigned int lx = this->_baseX + direction[this->_position][0];
	unsigned int ly = this->_baseY + direction[this->_position][1];
	return (this->_map[ly][lx]);
}

Goban::Case* GobanIterator::operator->()
{
	unsigned int lx = this->_baseX + direction[this->_position][0];
	unsigned int ly = this->_baseY + direction[this->_position][1];
	return (&this->_map[ly][lx]);
}

bool GobanIterator::isNull() const
{
	unsigned int lx = this->_baseX + direction[this->_position][0];
	unsigned int ly = this->_baseY + direction[this->_position][1];
		
	return lx >= this->_maxX || ly >= this->_maxY;
}
GobanIterator GobanIterator::makeEndIterator() const
{
	GobanIterator it(*this);
	it._position = 8;
	return it;
}

int GobanIterator::dir() const
{
	return this->_position;
}