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
	const float _boostsFactor = 1.5f;
	const float _baseSpeed = 200.f;
	const float _reach = 300.f;

	// Data
	Map* _map;
	Room* _room;

	sf::Vector2f _position, _direction, _size, _lookDirection;
	int _boosts;

	// Cosmetic
	sf::Texture _tex;
	sf::Sprite _sprite;

	// Movement
	bool _moving = false;
	sf::Vector2f _movTarget;
	sf::VertexArray _line;

	// Items
	float _range;

	// Weapons
	bool _hasWeapon;
	std::shared_ptr<Weapon> _activeWeapon;

#ifdef DEBUG
	sf::CircleShape _hitbox;
	sf::CircleShape _reachShape;
#endif

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void startMoving(const sf::Vector2f& target);
	bool reachedTarget();

	void shoot(const sf::Event& event);

public:
	Player(ResManager* res);

	void setMap(Map* map);
	
	// Item interaction
	void boost();
	bool pickupWeapon(const std::shared_ptr<Weapon>& weapon);
	void dropWeapon(const sf::Vector2f& mousePos);
	void setActiveWeaponNone();

	Weapon* activeWeapon();

	sf::Vector2f direction() const;

	sf::Vector2f position() const;
	float reach() const;

	sf::Vector2f finalPosition(const sf::Vector2f& mousePos) const;

	void updateEvent(const sf::Event& event);
	void update(float dt, const sf::Vector2f& mousePos);
};