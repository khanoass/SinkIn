#pragma once

#include <iostream>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "Debug.h"
#include "Entity.hpp"
#include "VeMa.hpp"
#include "Logger.hpp"

class Player; // To avoid circular reference problems

class Item : public Entity
{
private:
	// Data
	sf::Vector2f _position, _size, _player;
	bool _alive = true;
	float _range;

	// Cosmetic
	sf::Sprite _sprite;

#ifdef DEBUG
	sf::CircleShape _hitbox;
#endif

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (_alive)
		{
			target.draw(_sprite);
#ifdef DEBUG
			target.draw(_hitbox);
#endif
		}
	}

protected:
	virtual void picked(Player* player)
	{
		if (_alive) 
			Logger::log({ "Item picked" });
	}

public:
	Item(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Texture& texture, float range)
	{
		_position = position;
		_size = size;

		_sprite.setOrigin(_size.x / 2, _size.y / 2);
		_sprite.setPosition(_position);
		_sprite.setScale({ 2.f, 2.f });
		_sprite.setTexture(texture);

		_range = range;

#ifdef DEBUG
		_hitbox.setFillColor(sf::Color::Transparent);
		_hitbox.setOutlineColor(sf::Color::Red);
		_hitbox.setRadius(_range);
		_hitbox.setOrigin(_range, _range);
		_hitbox.setOutlineThickness(2.f);
		_hitbox.setPosition(_position);
#endif
	}

	sf::Vector2f position() const
	{
		return _position;
	}

	sf::Vector2f size() const
	{
		return _size;
	}

	float range() const
	{
		return _range;
	}

	bool alive() const
	{
		return _alive;
	}

	void pick(Player* player)
	{
		picked(player);
		_alive = false;
	}

	virtual void updateEvent(const sf::Event& event)
	{
	}

	virtual void update(float dt, const sf::Vector2f& mousePos)
	{
	}
};