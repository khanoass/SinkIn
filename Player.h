#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "Debug.h"
#include "VeMa.hpp"
#include "Ephemereal.hpp"

class Map;
class Room;

class Player : public Entity
{
private:
	const float _boostsFactor = 1.5f;
	const float _baseSpeed = 100.f;

	// Data
	Map* _map;
	Room* _room;

	sf::Vector2f _position, _direction, _size;
	int _boosts;

	// Cosmetic
	sf::Texture _tex;
	sf::Sprite _sprite;

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

	void updateEvent(const sf::Event& event);
	void update(float dt, const sf::Vector2f& mousePos);
};