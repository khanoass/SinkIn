#pragma once

#include <SFML/Graphics/View.hpp>

class Camera
{
private:
	// Smooth camera settings
	const float _camFactor = 1.75f;
	const float _clampThreshold = 0.2;

	// Data
	sf::View _view;

public:
	Camera(const sf::Vector2u& size, float zoom = 1.f)
	{
		_view = sf::View(sf::FloatRect(0, 0, (float)size.x, (float)size.y));
		_view.zoom(zoom);
	}

	void updatePlayerSmooth(float dt, const sf::Vector2f& playerPos, bool changedRoom)
	{
		sf::Vector2f movement = playerPos - _view.getCenter();
		if (vm::norm(movement) < _clampThreshold)
			_view.setCenter(playerPos);
		else if (changedRoom)
			_view.setCenter(playerPos); // TODO: Change to match direction
		else
			_view.setCenter(_view.getCenter() + (movement * dt * _camFactor));
	}

	sf::View& view()
	{
		return _view;
	}
};