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

class Map;
class Room;

class Player : public LiveEntity
{
private:
	const float _boostsFactor = 1.5f;
	const float _baseSpeed = 200.f;
	const float _reach = 300.f;

	// Data
	Map* _map;
	Room* _room;

	sf::Vector2f _position, _direction, _size;
	int _boosts;

	// Cosmetic
	sf::Texture _tex;
	sf::Sprite _sprite;
	sf::CircleShape _reachShape;

	// Movement
	bool _moving = false;
	sf::Vector2f _movTarget;
	sf::VertexArray _line;

	// Attack
	Ephemereal _attackEph;
	sf::Texture _attackSheet;

	// Items
	float _range;

#ifdef DEBUG
	sf::CircleShape _hitbox;
#endif

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void startMoving(const sf::Vector2f& target);
	bool reachedTarget();

public:
	Player();

	void setMap(Map* map);
	void boost();

	sf::Vector2f position() const;
	float reach() const;

	sf::Vector2f finalPosition(const sf::Vector2f& mousePos) const;

	void updateEvent(const sf::Event& event);
	void update(float dt, const sf::Vector2f& mousePos);
};