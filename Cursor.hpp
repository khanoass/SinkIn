#pragma once

#include "LiveEntity.hpp"
#include "Ephemereal.hpp"

class Cursor : public LiveEntity
{
private:
	// Data
	sf::Vector2f _size;
	Map* _map;
	std::shared_ptr<Player> _player;
	Ephemereal _eph;

	// Cosmetic
	sf::Sprite _sprite;
	sf::Texture* _y;
	sf::Texture* _n;
	sf::Texture* _d;
	sf::Texture* _ephSheet;
	bool _canMove;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(_eph, states);
		target.draw(_sprite, states);
	}

public:
	Cursor(Map* map, const std::shared_ptr<Player>& player, ResManager* res)
	{
		_size = { 32, 32 };

		_map = map;
		_player = player;

		_sprite.setOrigin({ _size.x / 2, _size.y / 2 });
		sf::Vector2i pos = sf::Mouse::getPosition();
		_sprite.setPosition((float)pos.x, (float)pos.y);
		_sprite.setScale(3.f, 3.f);

		_y = &res->textures.cursor_yes;
		_n = &res->textures.cursor_no;
		_d = &res->textures.cursor_door;
		_ephSheet = &res->textures.eph;

		_sprite.setTexture(*_y);
		_canMove = true;
	}

	sf::Vector2f finalPosition() const
	{
		return _sprite.getPosition();
	}

	void updateEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2f point = { (float)event.mouseButton.x, (float)event.mouseButton.y };
			sf::Vector2f final = _player->finalCursorPosition(point);
			auto r = _map->currentRoom();
			Direction d = None;
			if(r->pointInRoom(final) || r->pointInDoor(final, d))
				_eph.spawn(final, { 64, 64 }, _ephSheet, { 5, 2 }, 0.05f);
		}
	}

	virtual void update(float dt, const sf::Vector2f& mousePos) override
	{
		auto final = _player->finalCursorPosition(mousePos);
		_sprite.setPosition(final);

		std::shared_ptr<Room> r = _map->currentRoom();
		Direction dir = None;

		if (_player->pointInPlayer(final))		_sprite.setTexture(*_n);
		else if (r->pointInDoor(final, dir))	_sprite.setTexture(*_d);
		else									_sprite.setTexture(*_y);

		_eph.update(dt, mousePos);
	}
};