#pragma once

#include "Map.h"
#include "Cursor.hpp"
#include "Enemies.h"

class Level : public LiveEntity
{
private:
	// Data
	sf::Vector2f _screenCenter;

	std::shared_ptr<Map> _map;
	std::shared_ptr<Cursor> _cursor;
	std::shared_ptr<Player> _player;
	std::shared_ptr<Items> _items;
	std::shared_ptr<Enemies> _enemies;
	std::shared_ptr<Bullets> _bullets;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(*_map, states);
		target.draw(*_cursor, states);
	}

public:
	Level(const std::string& mapfilename, const sf::Vector2f& screenCenter, ResManager* res)
	{
		_screenCenter = screenCenter;

		// Init all level's stuff
		_map = std::make_shared<Map>(mapfilename, _screenCenter, res);
		_player = std::make_shared<Player>(res);
		_cursor = std::make_shared<Cursor>(_player, res);
		_items = std::make_shared<Items>();
		_bullets = std::make_shared<Bullets>();
		_enemies = std::make_shared<Enemies>(_player, _bullets, _items);
	}

	void start()
	{
		// Init map
		_map->setPlayer(_player);
		_map->setContents(_items, _enemies, _bullets);
		_map->generate();

		// Pass pointers to other pointers
		_player->setMap(_map);
		_enemies->setMap(_map);
		_cursor->setMap(_map);
		_cursor->setEnemies(_enemies);
	}

	std::shared_ptr<Player> player()
	{
		return _player;
	}

	std::shared_ptr<Map> map()
	{
		return _map;
	}

	void updateEvent(const sf::Event& event, float dt, const sf::Vector2f& mousePos)
	{
		_player->updateEvent(event, dt, mousePos);
	}

	virtual void update(float dt, const sf::Vector2f& mousePos)
	{
		_map->update(dt, mousePos);
		_items->update(dt, mousePos);
		_enemies->update(dt, mousePos);
		_bullets->update(dt, mousePos);
		_cursor->update(dt, mousePos);
		_player->update(dt, mousePos);
	}
};