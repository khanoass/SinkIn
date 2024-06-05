#pragma once

#include "Levels.hpp"
#include "Enemies.h"

class Cursor : public LiveEntity
{
private:
	// Data
	sf::Vector2f _size;
	std::shared_ptr<Player> _player;
	std::shared_ptr<Map> _map;
	std::shared_ptr<Enemies> _enemies;

	// Cosmetic
	sf::Sprite _sprite;
	sf::Texture* _y;
	sf::Texture* _n;
	sf::Texture* _d;
	sf::Texture* _ws;
	sf::Texture* _wn;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(_sprite, states);
	}

public:
	Cursor(const std::shared_ptr<Player>& player, ResManager* res)
	{
		_size = { 32, 32 };

		_player = player;
		_map = nullptr;

		_sprite.setOrigin({ _size.x / 2, _size.y / 2 });
		sf::Vector2i pos = sf::Mouse::getPosition();
		_sprite.setPosition((float)pos.x, (float)pos.y);
		_sprite.setScale(3.f, 3.f);

		_y = &res->textures.cursor_yes;
		_n = &res->textures.cursor_no;
		_d = &res->textures.cursor_door;
		_wn = &res->textures.cursor_weapon;
		_ws = &res->textures.cursor_ontarget;

		_sprite.setTexture(*_y);
	}

	void setMap(const std::shared_ptr<Map>& map)
	{
		_map = map;
	}

	void setEnemies(const std::shared_ptr<Enemies>& enemies)
	{
		_enemies = enemies;
	}

	sf::Vector2f finalPosition() const
	{
		return _sprite.getPosition();
	}

	virtual void update(float dt, const sf::Vector2f& mousePos) override
	{
		_sprite.setPosition(mousePos);

		if (_map == nullptr) return;

		std::shared_ptr<Room> r = _map->currentRoom();
		Direction dir = None;

		auto finalPos = mousePos - r->absoluteOffset();

		if (_player->pointInPlayer(finalPos))		_sprite.setTexture(*_n);	// Player
		else if (r->pointInDoor(finalPos, dir))		_sprite.setTexture(*_d);	// Door
		else if (_player->activeWeapon() != nullptr && _enemies != nullptr)							
		{
			auto en = _enemies->getAllInRoom(_map->currentRoom()->name());
			bool on = false;
			for (const auto& e : en)
			{
				if (vm::dist(e->position(), finalPos) < e->range())
				{
					on = true;
					break;
				}
			}
			if(on)					_sprite.setTexture(*_ws);	// Gun
			else					_sprite.setTexture(*_wn);	// Gun + Enemy
		}
		// Default
		else
			_sprite.setTexture(*_y);
	}
};