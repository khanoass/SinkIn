#pragma once

#include "Weapon.h"

class Rifle : public Weapon
{
public:
	Rifle(const sf::Vector2f& position, ResManager* res)
		: Weapon(position, { 50.f, 50.f }, { 25.f, 30.f }, { -40.f, 30.f }, { 0.f, -55.f }, &res->textures.rifle_ground, &res->textures.rifle_hold, &res->textures.muzzle, 32)
	{
		_mode = Mode::Burst3;
		_recoil = 250.f;
		_fireRate = 0.04f;
		_spread = 8.f;
		_bulletDamage = 18.f;
		_bulletAmount = 1;
		_bulletLifespan = 1.5f;
		_bulletSpeed = 1200.f;
		_bulletSize = { 5.f, 5.f };
		_dropDamage = 10.f;
	}
};