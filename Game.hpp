#pragma once

#include <SFML/Graphics/Shader.hpp>

#include "Map.h"
#include "Player.h"
#include "Cursor.hpp"
#include "GUI.hpp"

class Game : public LiveEntity
{
private:
	sf::Vector2f _center;

	Map _map;
	Player _player;
	Cursor _cursor;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(_map, states);
		target.draw(_cursor, states);
		target.draw(_player, states);
	}

public:
	Game(const sf::Vector2f& center) :
		_center(center),
		_map(Map("assets/maps/map1_normal.png", center)),
		_player(),
		_cursor(&_map, &_player)
	{
		_map.setPlayer(&_player);
		_map.generate();
		_player.setMap(&_map);
	}

	sf::Vector2f getPlayerPosition() const
	{
		return _player.position();
	}

	Map* getMapPtr()
	{
		return &_map;
	}

	void setVignettePosition(const sf::Vector2f& pos)
	{
		_map.getTexShaderPtr()->setUniform("playerPos", sf::Glsl::Vec2(pos));
	}

	virtual void updateEvent(const sf::Event& event)
	{
		_player.updateEvent(event);
		_cursor.updateEvent(event);
		_map.updateEvent(event);
	}

	virtual void update(float dt, const sf::Vector2f& mousePos)
	{
		_player.update(dt, mousePos);
		_cursor.update(dt, mousePos);
		_map.update(dt, mousePos);

		_map.getTexShaderPtr()->setUniform("screenSize", sf::Glsl::Vec2({ _center.x*2, _center.y*2 }));
		_map.getTexShaderPtr()->setUniform("radius", 0.5f);
	}
};