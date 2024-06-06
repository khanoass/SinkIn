#pragma once

#include <iostream>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "Debug.h"
#include "VeMa.hpp"
#include "Logger.hpp"
#include "ResManager.hpp"

class Player; // To avoid circular reference problems

class Item : public sf::Drawable
{
protected:
	// Const
	const float _range = 15.f;

	// Data
	sf::Vector2f _position, _size, _player;
	bool _alive = true;
	sf::Texture* _groundTexture;

	// Cosmetic
	sf::Sprite _sprite;

#ifdef DEBUG
	sf::CircleShape _hitbox;
#endif

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (_alive)
		{
			target.draw(_sprite, states);
#ifdef DEBUG
			target.draw(_hitbox, states);
#endif
		}
	}

	virtual void picked(const std::shared_ptr<Player>& player)
	{
		if (_alive) 
			Logger::log({ "Item picked" });
	}

public:
	Item(const sf::Vector2f& position, const sf::Vector2f& size, sf::Texture* groundTexture)
	{
		_position = position;
		_size = size;

		_groundTexture = groundTexture;
		_sprite.setOrigin(_size.x / 2, _size.y / 2);
		_sprite.setPosition(_position);
		_sprite.setScale({ 2.f, 2.f });
		_sprite.setTexture(*_groundTexture);

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
	
	void setGroundSettings(const sf::Vector2f& position, float angle)
	{
		_position = position;
		_sprite.setPosition(_position);
		_sprite.setRotation(angle);
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

	virtual void pick(const std::shared_ptr<Player>& player)
	{
		picked(player);
		_alive = false;
	}
};