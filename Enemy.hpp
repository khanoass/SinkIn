#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "Map.h"
#include "Entity.hpp"
#include "Debug.h"
#include "VeMa.hpp"
#include "Ephemereal.hpp"
#include "Weapon.h"
#include "ResManager.hpp"
#include "Bullets.hpp"
#include "Player.h"

class Enemy : public Entity
{
protected:
	const sf::Vector2f _scale = { 1.35f, 1.35f };
	const float _knockbackFactor = 100.f;

	// Data
	sf::Vector2f _startPosition;
	sf::Vector2f _position, _direction, _size;
	bool _alive = true;
	std::vector<sf::Vector2f> _border;
	bool _justHit = false;

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

	// Death animation
	Ephemereal _eph;
	sf::Texture* _ephSheet;

	// Must be defined by subclass!
	float _range, _hp, _speed, _friction, _damage;

	virtual void updateAI(float dt, const std::shared_ptr<Player>& player)
	{
	}

private:
#ifdef DEBUG
	sf::CircleShape _hitbox;
	sf::CircleShape _target;
#endif

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(_eph, states);

		if (_alive)
		{
			target.draw(_sprite, states);
#ifdef DEBUG
			target.draw(_hitbox);
			target.draw(_target);
#endif
		}
	}

	void die()
	{
		_eph.spawn(_position, { 64, 64 }, _ephSheet, { 5, 1 }, (float)0.05, 0);
		_alive = false;
	}

	void hit(float damage, const sf::Vector2f& direction)
	{
		_knockback = damage * _knockbackFactor;
		_knockbackDirection = direction;
		_hp -= damage;
		_knockedback = true;
		_justHit = true;

		Logger::log({ "-", std::to_string(damage), "hp, :", std::to_string(_hp) });

		if (_hp <= 0)
			die();
	}

protected:
	// Functions to be used by AI of subclass

	void startMoving(const sf::Vector2f& target)
	{
		_moving = true;
		_movTarget = target;
		_direction = vm::normalise((_movTarget - _position));
	}

	bool reachedTarget()
	{
		return vm::dist(_position, _movTarget) < _range;
	}

	bool hitPlayer(const std::shared_ptr<Player>& player)
	{
		return vm::dist(_position, player->position()) < _range + player->range();
	}

public:
	Enemy(const sf::Vector2f& position, const sf::Vector2f& size, sf::Texture* texture, sf::Texture* deathSheet)
	{
		_startPosition = position;
		_position = position;
		_size = size;
		_tex = texture;
		_ephSheet = deathSheet;

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
		_target.setRadius(2.f);
		_target.setFillColor(sf::Color::Red);
#endif
	}

	void resetPosition()
	{
		_position = _startPosition;
	}

	void setBorder(const sf::Vector2f& center, const sf::Vector2f& size)
	{
		_border = {
			sf::Vector2f(center.x - size.x / 2, center.x + size.x / 2),
			sf::Vector2f(center.y - size.y / 2, center.y + size.y / 2)
		};
	}

	sf::Vector2f position() const
	{
		return _position;
	}

	float range() const
	{
		return _range;
	}

	bool pointInEnemy(const sf::Vector2f& point) const
	{
		return vm::dist(_position, point) < _range;
	}

	void update(float dt, const sf::Vector2f& mousePos, const std::shared_ptr<Player>& player, std::vector<Bullet>& bullets, const std::vector<std::shared_ptr<Weapon>>* weapons)
	{
		_sprite.setRotation(vm::angle(_direction));
		_eph.update(dt, mousePos);

		if (!_alive) return;

		updateAI(dt, player);

		// Hit
		for (auto& b : bullets)
		{
			if (vm::dist(b.position, _position) < _range)
			{
				hit(b.damage, b.direction);
				b.dead = true;
			}
		}

		// Hit from dropped weapon
		if (weapons != nullptr)
		{
			for (const auto& w : *weapons)
			{
				if (w == nullptr) continue;
				if (w->dropping() && vm::dist(w->position(), _position) < _range + w->range())
				{
					w->bounce();
					hit(w->dropDamage(), w->dropDirection());
					_justHit = true;
				}
			}
		}

		// Hit player
		if (hitPlayer(player))
			player->hit(_damage, _direction);

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
				_position.x += _knockbackDirection.x * _knockback * dt;
				_position.y += _knockbackDirection.y * _knockback * dt;

				// Friction
				_knockback *= 1 / (1 + (dt * _friction));

				if (_knockback < 1)
					_knockedback = false;
			}

#ifdef DEBUG
			_hitbox.setPosition(_position);
			_target.setPosition(_movTarget);
#endif

			// Out-of-bounds checking
			sf::Vector2f old(_position);
			if (_position.x - _size.x / 2 < _border[0].x)
				_position.x = _border[0].x + _size.x / 2;
			if (_position.x + _size.x / 2 > _border[0].y)
				_position.x = _border[0].y - _size.x / 2;
			if (_position.y - _size.y / 2 < _border[1].x )
				_position.y = _border[1].x + _size.y / 2;
			if (_position.y + _size.y / 2 > _border[1].y)
				_position.y = _border[1].y - _size.y / 2;
			if (_position != old) _moving = false;

			_sprite.setPosition(_position);
			if (reachedTarget()) _moving = false;
		}
	}
};