#pragma once

enum Direction
{
	Up, Right, Down, Left, None
};

namespace Directions
{
	static void getHV(int& h, int& v, Direction dir)
	{
		h = 0;
		v = 0;
		switch (dir)
		{
		case Up:	v = -1; break;
		case Right:	h = 1;	break;
		case Down:	v = 1;	break;
		case Left:	h = -1; break;
		default: break;
		}
	}
}