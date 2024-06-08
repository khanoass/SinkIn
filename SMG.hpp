#pragma once

#include "Weapon.h"

class SMG : public Weapon
{
public:
	SMG(const sf::Vector2f& position, ResManager* res)
		: Weapon(position, { 50.f, 50.f }, { 25.f, 30.f }, { -40.f, 30.f }, { 0.f, -35.f }, &res->textures.smg_ground, &res->textures.smg_hold, &res->textures.muzzle, 75)
	{
		_mode =				Mode::Auto;
		_recoil =			100.f;
		_fireRate =			0.09f;
		_spread =			20.f;
		_bulletDamage =		6.f;
		_bulletAmount =		1;
		_bulletLifespan =	0.6f;
		_bulletSpeed =		1200.f;
		_bulletSize =		{ 5.f, 5.f };
		_dropDamage =		5.f;
	}
};