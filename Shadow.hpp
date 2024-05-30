#pragma once

#include "Enemy.hpp"

class Shadow : public Enemy
{
public:
	Shadow(const sf::Vector2f& position, ResManager* res)
		: Enemy(position, { 60, 60 }, &res->textures.shadow, &res->textures.death)
	{
		_hp = 100;
		_range = 50;
		_speed = 100;
		_friction = 0.92f;
		_damage = 10.f;
	}

	virtual void updateAI(const std::shared_ptr<Player>& player) override
	{
		startMoving(player->position());
	}
};