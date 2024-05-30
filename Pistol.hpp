#pragma once

#include "Weapon.h"

class Pistol : public Weapon
{
public:
	Pistol(const sf::Vector2f& position, ResManager* res)
		: Weapon(position, { 50, 50 }, { 25, 25 }, { -40, 30 }, { 0, -30 }, &res->textures.glock_ground, &res->textures.glock_hold, &res->textures.muzzle, 2000)
	{
		_bulletSize =		{ 5, 5 };
		_mode =				Mode::SemiAuto;
		_recoil =			{ 500 };
		_fireRate =			{ 0.5 };
		_bulletLifespan =	{ 0.3 };
		_bulletSpeed =		{ 1200 };
		_spread =			{ 10 };
		_bulletDamage =		{ 10 };
		_bulletAmount =		{ 1 };

		_dropSpeed = 2000;
		_dropRotationSpeed = 2000;
		_dropDamage = 10;
	}
};