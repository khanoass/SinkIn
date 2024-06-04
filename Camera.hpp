#pragma once

#include <SFML/Graphics/View.hpp>

class Camera
{
private:
	// Smooth camera settings
	const float _camFactor = 1.75f;
	const float _clampThreshold = 0.2f;

	// Data
	sf::View _view;

public:
	Camera(const sf::Vector2u& size, float zoom = 1.f)
	{
		_view = sf::View(sf::FloatRect(0, 0, (float)size.x, (float)size.y));
		_view.zoom(zoom);
	}

	sf::Vector2f playerSmoothCenter(float dt, const sf::Vector2f& playerPos, bool changedRoom)
	{
		sf::Vector2f movement = playerPos - _view.getCenter();
		if (vm::norm(movement) < _clampThreshold || changedRoom)
			return playerPos;
		return _view.getCenter() + (movement * dt * _camFactor);
	}

	void updatePlayerSmooth(float dt, const sf::Vector2f& playerPos, const sf::Vector2f& roomCenter, const sf::Vector2f& roomSize, bool changedRoom)
	{
		sf::Vector2f pos = playerPos;

		sf::Vector2f vhsize = { _view.getSize().x / 2, _view.getSize().y / 2 };
		sf::Vector2f rhsize = { roomSize.x / 2, roomSize.y / 2 };

		auto vmin = pos - vhsize;
		auto vmax = pos + vhsize;
		auto rmin = roomCenter - rhsize;
		auto rmax = roomCenter + rhsize;

		// Keep camera inside the room
		if (vhsize.x < rhsize.x)
		{
			if (vmin.x < rmin.x) pos.x = rmin.x + vhsize.x;
			if (vmax.x > rmax.x) pos.x = rmax.x - vhsize.x;
		}
		else
		{
			if (vmin.x > rmin.x) pos.x = rmin.x + vhsize.x;
			if (vmax.x < rmax.x) pos.x = rmax.x - vhsize.x;
		}

		if (vhsize.y < rhsize.y)
		{
			if (vmin.y < rmin.y) pos.y = rmin.y + vhsize.y;
			if (vmax.y > rmax.y) pos.y = rmax.y - vhsize.y;
		}
		else
		{
			if (vmin.y > rmin.y) pos.y = rmin.y + vhsize.y;
			if (vmax.y < rmax.y) pos.y = rmax.y - vhsize.y;
		}

		_view.setCenter(playerSmoothCenter(dt, pos, changedRoom));
	}

	sf::View& view()
	{
		return _view;
	}
};