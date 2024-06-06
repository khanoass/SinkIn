#pragma once

#include "Weapon.h"

class Shotgun : public Weapon
{
public:
	Shotgun(const sf::Vector2f& position, ResManager* res)
		: Weapon(position, { 50, 50 }, { 25, 30 }, { -40, 30 }, { 0, -55 }, &res->textures.shotgun_ground, &res->textures.shotgun_hold, &res->textures.muzzle, 6)
	{
		_bulletSize =		{ 5, 5 };
		_mode =				Mode::SemiAuto;
		_recoil =			{ 1000 };
		_fireRate =			{ 1.f };
		_bulletLifespan =	{ 0.3 };
		_bulletSpeed =		{ 1200 };
		_spread =			{ 30 };
		_bulletDamage =		{ 10 };
		_bulletAmount =		{ 6 };
		_dropDamage = 10;
	}
};