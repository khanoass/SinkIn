#pragma once

#include "Levels.hpp"

class Cursor : public LiveEntity
{
private:
	// Data
	sf::Vector2f _size;
	std::shared_ptr<Player> _player;

	// Cosmetic
	sf::Sprite _sprite;
	sf::Texture* _y;
	sf::Texture* _n;
	sf::Texture* _d;
	sf::Texture* _ws;
	sf::Texture* _wn;
	bool _canMove;
	Map* _map;

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
		_canMove = true;
	}

	void setMap(Map* map)
	{
		_map = map;
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

		if (_player->pointInPlayer(mousePos))		_sprite.setTexture(*_n);
		else if (r->pointInDoor(mousePos, dir))	_sprite.setTexture(*_d);
		else if (_player->activeWeapon() != nullptr)
		{
			//if()					_sprite.setTexture(*_ws); // Todo become red when on enemy
			/*else*/							_sprite.setTexture(*_wn);
		}
		else									_sprite.setTexture(*_y);
	}
};