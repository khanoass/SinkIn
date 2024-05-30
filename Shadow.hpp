#pragma once

#include "Enemy.hpp"

class Shadow : public Enemy
{
public:
	Shadow(const sf::Vector2f& position, ResManager* res)
		: Enemy(position, { 60, 60 }, &res->textures.shadow)
	{
		_hp = 100;
		_range = 50;
		_speed = 100;
	}

	virtual void updateAI()
	{

	}
};