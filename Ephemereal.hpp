#pragma once

#include <iostream>
#include <SFML/Graphics/Sprite.hpp>
#include "Entity.hpp"
#include "Animation.hpp"

class Ephemereal : public Entity
{
private:
	// Data
	sf::Vector2f _position, _size;
	sf::Clock _clock;
	bool _alive;

	// Cosmetic
	sf::Sprite _sprite;
	Animation _animation;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (_alive)
			target.draw(_sprite);
	}

public:
	Ephemereal()
	{
		_alive = false;
	}

	void spawn(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Texture& spritesheet, const sf::IntRect& frame, const sf::Vector2i& numFrames, float seconds)
	{
		_position = position;
		_size = size;
		_clock.restart();
		_alive = true;

		_animation.setSpritesheet(frame, numFrames, seconds);
		_animation.start(false);

		_sprite.setPosition(_position);
		_sprite.setOrigin(_size.x / 2, _size.y / 2);
		_sprite.setTexture(spritesheet);
		_sprite.setTextureRect(_animation.frame());
	}

	virtual void updateEvent(const sf::Event& event)
	{

	}

	virtual void update(float dt, const sf::Vector2f& mousePos)
	{
		if (_alive)
		{
			if (_animation.changed())
				_sprite.setTextureRect(_animation.frame());

			if (_animation.finished())
				_alive = false;
		}
	}
};