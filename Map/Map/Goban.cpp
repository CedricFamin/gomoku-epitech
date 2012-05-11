#include "Goban.h"

#define MIN(x, y) ((x < y) ? x : y)


void Goban::alloc_map(char **&map, unsigned int w, unsigned int h)
{
	map = new char*[h];
	for (unsigned int i = 0; i < h; i++)
	{
		map[i] = new char[w/4+1];
		for (unsigned int j = 0; j < w / 4 + 1; j++)
			map[i][j] = 0;
	}
}

Goban::Goban(unsigned int width, unsigned int height) : _width(width), _height(height)
{
	_map = new Case*[height];
	for (unsigned int i = 0; i < height; i++)
	{
		_map[i] = new Case[width];
		for (unsigned int j = 0; j < width; j++)
		{
			_map[i][j].rawData = 0;
			_map[i][j].free_case.rawData = 0xFF;
			if (i == 0)
			{
				_map[i][j].free_case.top = 0;
				_map[i][j].free_case.topLeft = 0;
				_map[i][j].free_case.topRight = 0;
			}
			else if (i == (this->_height - 1))
			{
				_map[i][j].free_case.bot = 0;
				_map[i][j].free_case.botLeft = 0;
				_map[i][j].free_case.botRight = 0;
			}
			if (j == 0)
			{
				_map[i][j].free_case.left = 0;
				_map[i][j].free_case.topLeft = 0;
				_map[i][j].free_case.botLeft = 0;
			}
			else if (j == (this->_width - 1))
			{
				_map[i][j].free_case.right = 0;
				_map[i][j].free_case.topRight = 0;
				_map[i][j].free_case.botRight = 0;
			}
		}
	}
}

Goban::Case ** Goban::GetMap()
{
	return this->_map;
}

void Goban::Putin(PION_TYPE type, unsigned int i, unsigned int j)
{
	Case & cCase = this->_map[j][i];

	cCase.color = type;
	_update_pair(cCase, i, j);
	_update_safe(cCase, i, j);
	_update_free(cCase, i, j);
}

void Goban::subIn(unsigned int i, unsigned int j)
{
	unsigned char pair = 0x10;

	TURN_AROUND(this->_map, i, j, this->_width, this->_height)
	{
		if (currentElem == 0)
			continue;
		currentElem->pair.rawData &= ~pair;
		currentElem->pair_safe.rawData &= ~pair;
		currentElem->free_case.rawData |= pair;
		pair <<= 1;
		if (pair == 0)
			pair = 0x1;
	}
	this->_map[j][i].color = EMPTY;
	this->_map[j][i].pair.rawData = 0;
	this->_map[j][i].pair_safe.rawData = 0;
}

void Goban::_update_safe(Case & cCase, unsigned int i, unsigned int j)
{
	int dir[8][2] = {
		{0,-3}, {3,-3}, {3,0}, {3,3},
		{0,3}, {-3,3}, {-3,0}, {-3,-3},
	};

	char bitdir = 0x1;
	TURN_AROUND(this->_map, i, j, this->_width, this->_height)
	{
		if (currentElem == 0)
		{
			bitdir <<=1;
			continue;
		}
		if ((currentElem->pair.rawData & bitdir))
		{
			unsigned int lx = i + dir[TURN_INDEX][0], ly = j + dir[TURN_INDEX][0];
			if (cCase.color == currentElem->color)
				currentElem->pair_safe.rawData |= bitdir;
			else if (this->InBound(lx, ly) &&
				this->_map[ly][lx].color == 0)
				currentElem->pair_safe.rawData &= ~bitdir;
		}
		bitdir <<=1;
	}
}

void Goban::_update_free(Case & cCase, unsigned int i, unsigned int j)
{
	unsigned char bitDir = 0x10;

	TURN_AROUND(this->_map, i, j, this->_width, this->_height)
	{
		if (currentElem)
			currentElem->free_case.rawData &= ~bitDir;
		bitDir <<=1;
		if (bitDir == 0)
			bitDir = 0x1;
	}

}

void Goban::_update_pair(Case & cCase, unsigned int i, unsigned int j)
{
	unsigned char pair = 0x1;
	unsigned char reverse_pair = 16;
	int dir[8][2] = {
		{0,-1}, {1,-1}, {1,0}, {1,1},
		{0,1}, {-1,1}, {-1,0}, {-1,-1},
	};

	TURN_AROUND(this->_map, i, j, this->_width, this->_height)
	{
		if (currentElem &&
			currentElem->color == cCase.color)
		{
			cCase.pair.rawData |= pair;
			currentElem->pair.rawData |= reverse_pair;
			unsigned int x1 = i - dir[TURN_INDEX][0];
			unsigned int y1 = j - dir[TURN_INDEX][1];
			unsigned int x = i + dir[TURN_INDEX][0] * 2;
			unsigned int y = j + dir[TURN_INDEX][1] * 2;
			if (!this->InBound(x1, y1) || !this->InBound(x, y) ||
				this->_map[y][x].color == cCase.color || this->_map[y1][x1].color == cCase.color ||
				(this->_map[y][x].color == EMPTY && this->_map[y1][x1].color == EMPTY))
			{
				cCase.pair_safe.rawData |= pair;
				currentElem->pair_safe.rawData |= reverse_pair;
			}

		}
		pair <<= 1;
		reverse_pair <<= 1;
		if (reverse_pair == 0)
			reverse_pair = 0x1;
	}
}


Goban::~Goban(void)
{
}

unsigned int Goban::getWidth() const
{
	return this->_width;
}

unsigned int Goban::getHeight() const
{
	return this->_height;
}

bool Goban::InBound(unsigned int x, unsigned int y) const
{
	return x < this->_width && y < this->_height;
}