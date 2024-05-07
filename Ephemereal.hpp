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

	void spawn(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Texture& spritesheet, const sf::Vector2i& numFrames, float seconds, float angle = 0)
	{
		_position = position;
		_size = size;
		_clock.restart();
		_alive = true;

		_animation.setSpritesheet(sf::IntRect({ 0, 0 }, { (int)_size.x, (int)_size.y }), numFrames, seconds);
		_animation.start(false);

		_sprite.setPosition(_position);
		_sprite.setOrigin(_size.x / 2, _size.y / 2);
		_sprite.setTexture(spritesheet);
		_sprite.setTextureRect(_animation.frame());
		_sprite.setRotation(angle);

#ifdef DEBUG
		std::cout << "Spawned eph" << std::endl;
#endif // DEBUG

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