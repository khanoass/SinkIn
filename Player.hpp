#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include "Entity.hpp"
#include "Ephemereal.hpp"
#include "VeMa.hpp"
#include <iostream>

class Player : public Entity
{
private:
	// Data
	Map* _map;
	Room* _room;

	sf::Vector2f _position, _direction, _size;
	float _baseSpeed;
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

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
#ifdef DEBUG
		if (_moving) target.draw(_line);
#endif
		target.draw(_sprite);
		target.draw(_attackEph);
	}

	void startMoving(const sf::Vector2f& target)
	{
		_moving = true;
		_movTarget = target;

		_line[0].position = _position;
		_line[1].position = _movTarget;

		_direction = vm::normalise((_movTarget - _position));
		_sprite.setRotation(vm::angle(_direction));
	}

	bool reachedTarget()
	{
		return vm::dist(_position, _movTarget) < 10.f;
	}

public:
	Player(Map* map) :
		_map(map)
	{
		_room = map->currentRoom();
		_position = _room->center();
		_baseSpeed = 100;
#ifdef DEBUG
		_baseSpeed *= 7;
#endif
		_boosts = 0;

		_tex.loadFromFile("assets/textures/placeholder_player.png");
		_size = { 60, 60 };

		_sprite.setOrigin({ _size.x / 2, _size.y / 2 });
		_sprite.setPosition(_room->center());
		_sprite.setTexture(_tex);

		_line.setPrimitiveType(sf::Lines);
		_line.append(sf::Vertex(_position, sf::Color::Red));
		_line.append(sf::Vertex(_position, sf::Color::Red));

		_attackSheet.loadFromFile("assets/textures/attack_2.png");
	}

	void updateEvent(const sf::Event& event)
	{
		// Start moving with click
		if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2f point = { (float)event.mouseButton.x, (float)event.mouseButton.y };

			if (_room->pointInRoom(point)) startMoving(point);

			Direction dir = None;
			if (_room->pointInDoor(point, dir)) startMoving(point);
		}

		// Attack
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
		{
			sf::Vector2f point = { (float)event.mouseButton.x, (float)event.mouseButton.y };
			sf::Vector2f dir = vm::normalise(point - _position);
			sf::Vector2f pos = _position + sf::Vector2f(dir.x * 50, dir.y * 50);
			float angle = vm::angle(dir);
			_attackEph.spawn(pos, { 64, 64 }, _attackSheet, { 5, 2 }, 0.05, angle);
			_sprite.setRotation(angle);
		}
	}

	void update(float dt, const sf::Vector2f& mousePos)
	{
		// Movement update
		if (_moving)
		{
			_position.x += _direction.x * _baseSpeed * dt;
			_position.y += _direction.y * _baseSpeed * dt;

			_line[0].position = _position;
			_sprite.setPosition(_position);
			if (reachedTarget()) _moving = false;
		}

		// Room exit
		Direction dir = None;
		if (_room->pointInDoor(_position, dir))
		{
			_map->exitRoom(dir);
			_room = _map->currentRoom();

			std::cout << "Player entered: " << _room->name() << std::endl;

			_moving = false;
			_position = _room->spawn(dir);	
			_sprite.setPosition(_position);
		}

		// Attack
		_attackEph.update(dt, mousePos);
	}
};