#pragma once

#include <SFML/Graphics/Shader.hpp>

#include "Map.h"
#include "Player.h"
#include "Cursor.hpp"
#include "GUI.hpp"

class Game : public LiveEntity
{
private:
	// Data
	sf::Vector2f _center;

	Map _map;
	std::shared_ptr<Player> _player;
	Cursor _cursor;
	std::shared_ptr<Items> _items;
	std::shared_ptr<Enemies> _enemies;
	std::shared_ptr<Bullets> _bullets;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(_map, states);
		target.draw(*_player, states);
		target.draw(_cursor, states);
	}

public:
	Game(const sf::Vector2f& center, ResManager* res) :
		_center(center),
		_map(Map("assets/maps/map1_normal.png", center, res)),
		_player(std::make_shared<Player>(res)),
		_cursor(&_map, _player, res)
	{
		_items = std::make_shared<Items>();
		_bullets = std::make_shared<Bullets>();
		_enemies = std::make_shared<Enemies>(_player, _bullets, _items);

		_map.setPlayer(_player);
		_map.setContents(_items, _enemies, _bullets);
		_map.generate();
		_player->setMap(&_map);
	}

	sf::Vector2f getPlayerPosition() const
	{
		return _player->position();
	}

	int getPlayerAmmo() const
	{
		return _player->ammo();
	}

	bool playerHasWeapon() const
	{
		return _player->activeWeapon() != nullptr;
	}

	Map* getMapPtr()
	{
		return &_map;
	}

	void setVignettePosition(const sf::Vector2f& pos)
	{
		_map.getTexShaderPtr()->setUniform("playerPos", sf::Glsl::Vec2(pos));
	}

	void updateEvent(const sf::Event& event, float dt, const sf::Vector2f& mousePos)
	{
		_player->updateEvent(event, dt, mousePos);
	}

	virtual void update(float dt, const sf::Vector2f& mousePos) override
	{
		_cursor.update(dt, mousePos);
		_player->update(dt, _cursor.finalPosition());
		_map.update(dt, _cursor.finalPosition());
		_items->update(dt, mousePos);
		_enemies->update(dt, mousePos);
		_bullets->update(dt, mousePos);

		_map.getTexShaderPtr()->setUniform("screenSize", sf::Glsl::Vec2({ _center.x*2, _center.y*2 }));
		_map.getTexShaderPtr()->setUniform("radius", 0.5f);
	}
};