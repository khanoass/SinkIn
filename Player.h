#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "LiveEntity.hpp"
#include "Debug.h"
#include "VeMa.hpp"
#include "Ephemereal.hpp"
#include "Weapon.h"
#include "ResManager.hpp"

class Map;
class Room;

class Player : public LiveEntity, public std::enable_shared_from_this<Player>
{
private:
	const sf::Vector2f _size = { 60, 60 };
	const sf::Vector2f _scale = { 1.35f, 1.35f };
	const float _boostsFactor = 1.5f;
	const float _baseSpeed = 200.f;
	const float _reach = 300.f;
	const float _range = 35.f;
	const float _friction = 0.95f;
	const float _knockbackDamageFactor = 100.f;
	const float _hitCooldown = 3.f;
	const float _hitVisSpeed = 0.1f;
	const float _initialHp = 1000.f;

	// Data
	Map* _map;
	std::shared_ptr<Room> _room;

	sf::Vector2f _position, _direction, _lookDirection;
	int _boosts;
	float _hp;
	bool _hit = false;
	bool _hitVis = false;
	sf::Clock _hitClock, _hitVisClock;

	// Cosmetic
	sf::Texture _tex;
	sf::Sprite _sprite;

	// Movement
	bool _moving = false;
	sf::Vector2f _movTarget;
	sf::VertexArray _line;

	// Knockback
	bool _knockedback = false;
	sf::Vector2f _knockbackDirection;
	float _knockback;

	// Weapons
	std::shared_ptr<Weapon> _weapon;

#ifdef DEBUG
	sf::CircleShape _hitbox;
	sf::CircleShape _reachShape;
#endif

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void startMoving(const sf::Vector2f& target);
	bool reachedTarget();

	void shoot(const sf::Event& event);

	void die();

public:
	Player(ResManager* res);

	void setMap(Map* map);
	
	// Item interaction
	void boost();
	bool pickupWeapon(const std::shared_ptr<Weapon>& weapon);
	void dropWeapon(const sf::Vector2f& mousePos);
	void setKnockback(float knockback, const sf::Vector2f& direction);
	void hit(float damage, const sf::Vector2f& direction);

	std::shared_ptr<Weapon> activeWeapon();

	// Get
	sf::Vector2f direction() const;
	sf::Vector2f position() const;
	float reach() const;
	float range() const;
	int ammo() const;

	// Util
	bool pointInPlayer(const sf::Vector2f& point) const;
	sf::Vector2f finalCursorPosition(const sf::Vector2f& mousePos) const;

	void updateEvent(const sf::Event& event, float dt, const sf::Vector2f& mousePos);
	void update(float dt, const sf::Vector2f& mousePos) override;
};