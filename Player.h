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
class Level;

class Player : public LiveEntity, public std::enable_shared_from_this<Player>
{
private:
	const sf::Vector2f _size = { 60.f, 60.f };
	const sf::Vector2f _scale = { 1.35f, 1.35f };
	const float _boostsFactor = 1.5f;
	const float _boostsTime = 20.f;
	const float _baseSpeed = 200.f;
	const float _range = 35.f;
	const float _friction = 5.f;
	const float _knockbackDamageFactor = 50.f;
	const float _hitCooldown = 3.f;
	const float _hitVisSpeed = 0.1f;
	const float _initialHp = 100.f;

	// Data
	std::shared_ptr<Map> _map;
	std::shared_ptr<Room> _room;
	std::shared_ptr<Level> _level;

	sf::Vector2f _position, _direction, _lookDirection;
	int _boosts;
	float _hp;
	bool _hit = false;
	bool _hitVis = false;
	bool _boosting = false;
	sf::Clock _hitClock, _hitVisClock, _boostClock;

	// Cosmetic
	sf::Texture _tex;
	sf::Sprite _sprite;

	// Movement
	bool _moving = false;

	// Knockback
	bool _knockedback = false;
	sf::Vector2f _knockbackDirection;
	float _knockback;

	// Weapons
	std::shared_ptr<Weapon> _weapon;

#ifdef DEBUG
	sf::CircleShape _hitbox;
#endif

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Vector2f getMovingVector();

	void shoot(const sf::Vector2f& mousePos);

	void die();

public:
	Player(ResManager* res);

	void setMap(const std::shared_ptr<Map>& map);
	void setLevel(const std::shared_ptr<Level>& level);
	
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
	sf::Vector2f absolutePosition() const;
	float range() const;
	int ammo() const;
	float boostTime() const;
	float maxBoostTime() const;
	float hp() const;
	float maxHp() const;
	bool boosted() const;

	// Util
	bool pointInPlayer(const sf::Vector2f& point) const;

	void updateEvent(const sf::Event& event, float dt, const sf::Vector2f& mousePos);
	void update(float dt, const sf::Vector2f& mousePos) override;
};