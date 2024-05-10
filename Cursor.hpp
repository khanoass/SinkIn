#pragma once

#include "LiveEntity.hpp"
#include "Ephemereal.hpp"

class Cursor : public LiveEntity
{
private:
	// Data
	sf::Vector2f _size;
	Map* _map;
	Player* _player;
	Ephemereal _eph;

	// Cosmetic
	sf::Sprite _sprite;
	sf::Texture _f1, _f2, _f3;
	bool _canMove;
	sf::Texture _ephSheet;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(_eph, states);
		target.draw(_sprite, states);
	}

public:
	Cursor(Map* map, Player* player)
	{
		_size = { 32, 32 };

		_map = map;
		_player = player;

		_sprite.setOrigin({ _size.x / 2, _size.y / 2 });
		sf::Vector2i pos = sf::Mouse::getPosition();
		_sprite.setPosition((float)pos.x, (float)pos.y);
		_sprite.setScale(3.f, 3.f);

		_f1.loadFromFile("assets/textures/f1.png");
		_f2.loadFromFile("assets/textures/f2.png");
		_f3.loadFromFile("assets/textures/f3.png");

		_ephSheet.loadFromFile("assets/textures/eph.png");

		_sprite.setTexture(_f1);

		_canMove = true;
	}

	virtual void updateEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2f point = { (float)event.mouseButton.x, (float)event.mouseButton.y };
			sf::Vector2f final = _player->finalPosition(point);
			auto r = _map->currentRoom();
			Direction d = None;
			if(r->pointInRoom(final) || r->pointInDoor(final, d))
				_eph.spawn(final, { 64, 64 }, _ephSheet, { 5, 2 }, 0.05f);
		}
	}

	virtual void update(float dt, const sf::Vector2f& mousePos)
	{
		auto final = _player->finalPosition(mousePos);
		_sprite.setPosition(final);

		Room* r = _map->currentRoom();
		Direction dir = None;

		if (r->pointInRoom(final))				_sprite.setTexture(_f1);
		else if (r->pointInDoor(final, dir))	_sprite.setTexture(_f3);
		else									_sprite.setTexture(_f2);

		_eph.update(dt, mousePos);
	}
};