#pragma once

#include <string>

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

	static Direction opposite(Direction dir)
	{
		switch (dir)
		{
		case Up:	return Down;
		case Right:	return Left;
		case Down:	return Up;
		case Left:	return Right;
		default: return None;
		}
	}

	static std::string to_str(Direction dir)
	{
		switch (dir)
		{
		case Up:	return "Up";
		case Right:	return "Right";
		case Down:	return "Down";
		case Left:	return "Left";
		default: return "None";
		}
	}
}