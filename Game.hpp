#pragma once

#include "Map.hpp"
#include "Player.hpp"
#include "Cursor.hpp"

class Game : public Entity
{
private:
	const sf::Vector2f center = { 600, 400 };

	Map _map;
	Player _player;
	Cursor _cursor;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(_map);
		target.draw(_cursor);
		target.draw(_player);
	}

public:
	Game() :
		_map(Map("assets/maps/map1.png", center)),
		_player(&_map),
		_cursor(&_map)
	{
	}

	virtual void updateEvent(const sf::Event& event)
	{
		_map.updateEvent(event);
		_player.updateEvent(event);
		_cursor.updateEvent(event);
	}

	virtual void update(float dt, const sf::Vector2f& mousePos)
	{
		_map.update(dt, mousePos);
		_player.update(dt, mousePos);
		_cursor.update(dt, mousePos);
	}
};