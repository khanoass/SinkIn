#pragma once

#include "Weapon.h"

class Pistol : public Weapon
{
public:
	Pistol(const sf::Vector2f& position, ResManager* res)
		: Weapon(position, { 50, 50 }, { 25, 25 }, { -40, 30 }, { 0, -30 }, &res->textures.glock_ground, &res->textures.glock_hold, &res->textures.muzzle, 12)
	{
		_bulletSize =		{ 5, 5 };
		_mode =				Mode::SemiAuto;
		_recoil =			{ 300.f };
		_fireRate =			{ 0.5f };
		_bulletLifespan =	{ 1.5f };
		_bulletSpeed =		{ 1200.f };
		_spread =			{ 10.f };
		_bulletDamage =		{ 12.f };
		_bulletAmount =		{ 1 };
		_dropDamage = 5.f;
	}
};