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
	sf::Texture* _f1;
	sf::Texture* _f2;
	sf::Texture* _f3;
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

		_f1 = &res->textures.cursor_yes;
		_f2 = &res->textures.cursor_no;
		_f3 = &res->textures.cursor_door;
		_ephSheet = &res->textures.eph;

		_sprite.setTexture(*_f1);
		_canMove = true;
	}

	sf::Vector2f finalPosition() const
	{
		return _sprite.getPosition();
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

		if (r->pointInRoom(final) &&
			!_player->pointInPlayer(final))		_sprite.setTexture(*_f1);
		else if (r->pointInDoor(final, dir))	_sprite.setTexture(*_f3);
		else									_sprite.setTexture(*_f2);

		_eph.update(dt, mousePos);
	}
};