#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "Map.h"
#include "Debug.h"
#include "VeMa.hpp"
#include "Ephemereal.hpp"
#include "Weapon.h"
#include "ResManager.hpp"
#include "Bullets.hpp"
#include "Player.h"
#include "Heart.h"

class Enemy : public sf::Drawable
{
protected:
	const sf::Vector2f _scale = { 1.35f, 1.35f };
	const float _knockbackFactor = 100.f;

	// Data
	sf::Vector2f _startPosition;
	sf::Vector2f _position, _direction, _size;
	bool _alive = true;
	std::vector<sf::Vector2f> _border;
	std::string _roomname;
	bool _justHit = false;
	bool _justTouched = false;
	std::shared_ptr<Weapon> _justHitDrop = nullptr;
	float _hp, _initialHp;
	ResManager* _res;
	std::shared_ptr<Items> _items;

	// Cosmetic
	sf::RenderTexture _tex;
	sf::Sprite _sprite;

	// Movement
	bool _moving = false;
	sf::Vector2f _movTarget;

	// Knockback
	bool _knockedback = false;
	sf::Vector2f _knockbackDirection;
	float _knockback;

	// Impulse
	bool _impulsing = false;
	sf::Vector2f _impulseDirection;
	float _impulse;

	// Death animation
	Ephemereal _eph;
	sf::Texture* _ephSheet;
	sf::Vector2i _frames;
	sf::Vector2f _frameSize;

	// Lifebar
	sf::VertexArray _bar;
	const sf::Color _bg = { 100, 30, 30, 140 };
	const sf::Color _fg = { 240, 50, 50, 140 };
	const sf::Vector2f _barSize = { 100, 10 };
	const sf::Vector2f _barPadding = { 2, 2 };
	const sf::Vector2f _barMargin = { -50, -70 };

	const std::vector<sf::Vector2f> _barOffsets = {
		{ 0, 0 },
		{ _barSize.x, 0 },
		{ _barSize.x, _barSize.y },
		{ 0, _barSize.y },
		{ _barPadding.x, _barPadding.y },
		{ _barSize.x - _barPadding.x, _barPadding.y },
		{ _barSize.x - _barPadding.x, _barSize.y - _barPadding.y },
		{ _barPadding.x, _barSize.y - _barPadding.y }
	};

	// Must be defined by subclass!
	float _range, _speed, _friction, _damage;

	virtual void updateEnemy(float dt, const std::shared_ptr<Player>& player)
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
			target.draw(_bar, states);
#ifdef DEBUG
			target.draw(_hitbox);
			target.draw(_target);
#endif
		}
	}

	void die()
	{
		_eph.spawn(_position, _frameSize, _ephSheet, _frames, (float)0.05, 0);
		_alive = false;
		
		// Drop heart
		_items->add(std::make_shared<Heart>(_position, _res), _roomname);
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

	void updateBar()
	{
		for (int i = 0; i < _bar.getVertexCount(); i++)
			_bar[i].position = _position + _barOffsets[i] + _barMargin;
			
		_bar[5].position.x = _position.x + _barMargin.x + (_hp / _initialHp) * _barSize.x - _barPadding.x * 2;
		_bar[6].position.x = _position.x + _barMargin.x + (_hp / _initialHp) * _barSize.x - _barPadding.x * 2;
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
		_justTouched = vm::dist(_position, player->position()) < _range + player->range();
		/*sf::Vector2f v = _position - player->position();
		auto d = vm::norm(v);
		float combinedRange = player->range() + _range;
		if (d < combinedRange)
		{
			auto p = player->position();
			sf::Vector2f n = vm::normalise(v);
			float penetrationDepth = combinedRange - d;
			p.x -= n.x * penetrationDepth / 2;
			p.y -= n.y * penetrationDepth / 2;
			_position.x += n.x * penetrationDepth / 2;
			_position.y += n.y * penetrationDepth / 2;
			player->setPosition(p);
			_sprite.setPosition(_position);
		}*/
		return _justTouched;
	}

	void applyImpulse(float force)
	{
		_impulse = force;
		_impulsing = true;
		_impulseDirection = _direction;
	}

public:
	Enemy(const sf::Vector2f& position, const sf::Vector2f& size, sf::Texture* deathSheet, const sf::Vector2i& deathFrames, const sf::Vector2f& deathFrameSize, float hp, ResManager* res)
	{
		_startPosition = position;
		_position = position;
		_size = size;
		_ephSheet = deathSheet;
		_frames = deathFrames;
		_frameSize = deathFrameSize;
		_hp = hp;
		_initialHp = hp;
		_res = res;

		_tex.create((unsigned int)size.x, (unsigned int)size.y);
		_sprite.setTexture(_tex.getTexture());
		_sprite.setOrigin({ size.x / 2, size.y / 2 });
		_sprite.setPosition(_position);
		_sprite.setScale(_scale);

		_bar.setPrimitiveType(sf::Quads);
		for(int i = 0; i < 4; i++) _bar.append(sf::Vertex({ 0, 0 }, _bg));
		for(int i = 0; i < 4; i++) _bar.append(sf::Vertex({ 0, 0 }, _fg));
		updateBar();

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

	void setRoomParams(const sf::Vector2f& center, const sf::Vector2f& size, const std::string& name)
	{
		_roomname = name;
		_border = {
			sf::Vector2f(center.x - size.x / 2, center.x + size.x / 2),
			sf::Vector2f(center.y - size.y / 2, center.y + size.y / 2)
		};
	}

	void setItems(const std::shared_ptr<Items> items)
	{
		_items = items;
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

	void update(float dt, const sf::Vector2f& mousePos, const std::shared_ptr<Player>& player, std::vector<Bullet>& bullets, const std::vector<std::shared_ptr<Weapon>>* weapons, const std::vector<std::shared_ptr<Enemy>>* population)
	{
		_sprite.setRotation(vm::angle(_direction));
		_eph.update(dt, mousePos);
		updateBar();

		if (!_alive) return;

		updateEnemy(dt, player);

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
				if (w == nullptr || !w->dropping()) continue;
				if (vm::dist(w->position(), _position) < _range + w->range())
				{
					if (_justHitDrop == nullptr)
					{
						w->bounce();
						hit(w->dropDamage(), w->dropDirection());
						_justHitDrop = w;
						Logger::log({ "Hit from dropped" });
					}
				}
				else if(_justHitDrop == w)
				{
					_justHitDrop = nullptr;
				}
			}
		}

		// Hit player
		if (hitPlayer(player))
			player->hit(_damage, _direction);

		// Movement update
		if (_moving || _knockedback || _impulsing)
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

			if (_impulsing)
			{
				_position.x += _impulseDirection.x * _impulse * dt;
				_position.y += _impulseDirection.y * _impulse * dt;

				// Friction
				_impulse *= 1 / (1 + (dt * _friction));
				if (_impulse < 1)
					_impulsing = false;
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

			// Collision with other enemies
			for (auto& i : *population)
			{
				if (i.get() == this) continue;
				sf::Vector2f v = _position - i->_position;
				auto d = vm::norm(v);
				float combinedRange = i->_range + _range;
				if (d < combinedRange)
				{
					sf::Vector2f n = vm::normalise(v);
					float penetrationDepth = combinedRange - d;
					i->_position.x -= n.x * penetrationDepth / 2;
					i->_position.y -= n.y * penetrationDepth / 2;
					_position.x += n.x * penetrationDepth / 2;
					_position.y += n.y * penetrationDepth / 2;
					i->_sprite.setPosition(i->_position);
				}
			}

			_sprite.setPosition(_position);
			if (reachedTarget()) _moving = false;
		}
	}
};