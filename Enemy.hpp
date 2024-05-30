#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "Entity.hpp"
#include "Debug.h"
#include "VeMa.hpp"
#include "Ephemereal.hpp"
#include "Weapon.h"
#include "ResManager.hpp"
#include "Bullets.hpp"

class Enemy : public Entity
{
private:
	const sf::Vector2f _scale = { 1.35f, 1.35f };
	const float _knockbackFactor = 20.f;

	// Data
	sf::Vector2f _position, _direction, _size;
	bool _alive = true;

	// Cosmetic
	sf::Texture* _tex;
	sf::Sprite _sprite;

	// Movement
	bool _moving = false;
	sf::Vector2f _movTarget;

	// Knockback
	bool _knockedback = false;
	sf::Vector2f _knockbackDirection;
	float _knockback;

protected:
	// Must be defined by subclass!
	float _range, _hp, _speed, _friction;

	virtual void updateAI(const std::shared_ptr<Player>& player)
	{
	}

private:
#ifdef DEBUG
	sf::CircleShape _hitbox;
#endif

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

	void hit(float damage, const sf::Vector2f& direction)
	{
		_knockback = damage * _knockbackFactor;
		_knockbackDirection = direction;
		_hp -= damage;
		_knockedback = true;
	}

	void startMoving(const sf::Vector2f& target)
	{
		_moving = true;
		_movTarget = target;
		_direction = vm::normalise((_movTarget - _position));
		_sprite.setRotation(vm::angle(_direction));
	}

	bool reachedTarget()
	{
		return vm::dist(_position, _movTarget) < 10.f;
	}

public:
	Enemy(const sf::Vector2f& position, const sf::Vector2f& size, sf::Texture* texture)
	{
		_position = position;
		_size = size;
		_tex = texture;

		_sprite.setOrigin({ size.x / 2, size.y / 2 });
		_sprite.setPosition(_position);
		_sprite.setScale(_scale);
		_sprite.setTexture(*_tex);

#ifdef DEBUG
		_hitbox.setFillColor(sf::Color::Transparent);
		_hitbox.setOutlineColor(sf::Color::Red);
		_hitbox.setRadius(_range);
		_hitbox.setOrigin(_range, _range);
		_hitbox.setOutlineThickness(2.f);
		_hitbox.setPosition(_position);
#endif
	}

	virtual void update(float dt, const sf::Vector2f& mousePos, const std::shared_ptr<Player>& player, std::vector<Bullet>& bullets)
	{
		updateAI(player);

		// Hit
		for (auto& b : bullets)
		{
			if (vm::dist(b.position, _position) < _range)
			{
				hit(b.damage, b.direction);
				b.dead = true;
			}
		}

		// Movement update
		if (_moving || _knockedback)
		{
			if (_moving)
			{
				_position.x += _direction.x * _speed * dt;
				_position.y += _direction.y * _speed * dt;
			}

			if (_knockedback)
			{
				_moving = false;

				_position.x += _knockbackDirection.x * _knockback * dt;
				_position.y += _knockbackDirection.y * _knockback * dt;
				_knockback *= _friction;

				if (_knockback < 1)
					_knockedback = false;
			}

#ifdef DEBUG
			_hitbox.setPosition(_position);
#endif

			_sprite.setPosition(_position);
			if (reachedTarget()) _moving = false;
		}
	}
};