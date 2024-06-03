#pragma once

#include "Weapon.h"

class SMG : public Weapon
{
public:
	SMG(const sf::Vector2f& position, ResManager* res)
		: Weapon(position, { 50, 50 }, { 25, 30 }, { -40, 30 }, { 0, -35 }, &res->textures.smg_ground, &res->textures.smg_hold, &res->textures.muzzle, 2000)
	{
		_mode =				Mode::Auto;
		_recoil =			{ 100 };
		_fireRate =			{ 0.09 };
		_spread =			{ 20 };
		_bulletDamage =		{ 3 };
		_bulletAmount =		{ 1 };
		_bulletLifespan =	{ 0.3 };
		_bulletSpeed =		{ 1200 };
		_bulletSize =		{ 5, 5 };
		_dropDamage = 15;
	}
};