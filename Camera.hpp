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

	void updatePlayerSmooth(float dt, const sf::Vector2f& playerPos, bool changedRoom)
	{
		_view.setCenter(playerSmoothCenter(dt, playerPos, changedRoom));
	}

	sf::View& view()
	{
		return _view;
	}
};