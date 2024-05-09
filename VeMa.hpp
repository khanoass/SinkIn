#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace vm
{
	static double PI = 3.141592654;

	static float normSq(const sf::Vector2f& v)
	{
		return v.x * v.x + v.y * v.y;
	}

	static float norm(const sf::Vector2f& v)
	{
		return std::sqrt(normSq(v));
	}

	static float dist(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return norm(a - b);
	}

	static sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t)
	{
		return start + t * (end - start);
	}

	static sf::Vector2f normalise(const sf::Vector2f& v)
	{
		float length = std::sqrt(v.x * v.x + v.y * v.y);
		if (length != 0)	return sf::Vector2f(v.x / length, v.y / length);
		else				return sf::Vector2f(0, 0);
	}

	static float angle(const sf::Vector2f& v)
	{
		return std::atan2(v.y, v.x) * 180 / (float)PI - 90;
	}

	struct Vector2iComparator {
		bool operator() (const sf::Vector2i& lhs, const sf::Vector2i& rhs) const
		{	
			if (lhs.x < rhs.x)							return true;
			else if (lhs.x == rhs.x && lhs.y < rhs.y)	return true;
			return false;
		}
	};
}