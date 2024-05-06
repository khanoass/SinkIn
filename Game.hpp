#pragma once

#include "Map.hpp"
#include "Player.hpp"

class Game : public Entity
{
private:
	const sf::Vector2f center = { 600, 400 };

	Map _map;
	Player _player;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(_map);
		target.draw(_player);
	}

public:
	Game() :
		_map(Map("", center)),
		_player(&_map)
	{
	}

	virtual void updateEvent(const sf::Event& event)
	{
		_map.updateEvent(event);
		_player.updateEvent(event);
	}

	virtual void update(float dt)
	{
		_map.update(dt);
		_player.update(dt);
	}
};