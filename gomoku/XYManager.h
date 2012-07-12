#ifndef XYMANAGER_H_
# define XYMANAGER_H_

#include <qdebug>

template<int dir> struct XYManager { };

template<> struct XYManager<0>
{ 
	inline void init(int &x, int &y)
	{
		x = 0;
		y = 18;
	}
	inline bool ended(int &x, int &) { return x >= 19; }
	inline void incremente(int &x, int &y, int & add)
	{
		y -= add;
		if (y < 0)
		{
			x++;
			y = 18;
		}
	}
};

template<> struct XYManager<1>
{ 
	inline void init(int &x, int &y)
	{
		x = 0;
		y = 0;
		this->_line = 0;
	}
	inline bool ended(int &, int &) { return this->_line >= 37; }
	inline void incremente(int &x, int &y, int & add)
	{
		y -= add;
		x += add;
		if (x >= 19 || y < 0)
		{
			x = 0;
			y = ++this->_line;
			if (y >= 19)
			{
				x = y % 19;
				y = 18;
			}
		}
	}
private:
	int _line;
};

template<> struct XYManager<2>
{ 
	static inline void init(int &x, int &y)
	{
		x = 0;
		y = 0;
	}
	static inline bool ended(int &, int &y) { return y >= 19; }
	static inline void incremente(int &x, int &y, int & add)
	{
		x += add;
		if (x >= 19)
		{
			y++;
			x = 0;
		}
	}
};

template<> struct XYManager<3>
{ 
	inline void init(int &x, int &y)
	{
		x = 0;
		y = 18;
		this->_line = 0;
	}
	inline bool ended(int &, int &) { return this->_line >= 37; }
	inline void incremente(int &x, int &y, int & add)
	{
		y += add;
		x += add;
		if (x >= 19 || y >= 19)
		{
			y = 18 - ++this->_line;
			x = 0;
			if (y < 0)
			{
				x = -y;
				y = 0;
			}
		}
	}

private:
	int _line;
};

#endif