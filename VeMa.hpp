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

	static float dot(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	static float angleTwo(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		float d = dot(a, b);
		float det = a.x * b.y - a.y * b.x;
		return std::atan2(det, d) * 180 / (float)PI;
	}

	inline float degToRad(float degrees)
	{
		return degrees * (float)PI / 180.f;
	}

	static sf::Vector2f rotateVector(const sf::Vector2f& v, float angle)
	{
		float rad = degToRad(angle);
		float cosTheta = cos(rad);
		float sinTheta = sin(rad);

		return sf::Vector2f(
			cosTheta * v.x - sinTheta * v.y,
			sinTheta * v.x + cosTheta * v.y
		);
	}

	static bool contains(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Vector2f& point)
	{
		return
			point.x > pos.x &&
			point.x < pos.x + size.x &&
			point.y > pos.y &&
			point.y < pos.y + size.y;
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