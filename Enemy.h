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

class Enemy : public LiveEntity
{
private:
	// Data
	sf::Vector2f _position, _direction, _size;

	// Cosmetic
	sf::Texture _tex;
	sf::Sprite _sprite;

	// Knockback
	bool _knockedback = false;
	sf::Vector2f _knockbackDirection;
	float _knockback;

	// Player interaction
	float _range;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
	Enemy(ResManager* res);

	void update(float dt, const sf::Vector2f& mousePos) override;
};