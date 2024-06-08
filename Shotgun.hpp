#pragma once

#include "Weapon.h"

class Shotgun : public Weapon
{
public:
	Shotgun(const sf::Vector2f& position, ResManager* res)
		: Weapon(position, { 50.f, 50.f }, { 25.f, 30.f }, { -40.f, 30.f }, { 0.f, -55.f }, &res->textures.shotgun_ground, &res->textures.shotgun_hold, &res->textures.muzzle, 6)
	{
		_bulletSize =		{ 5.f, 5.f };
		_mode =				Mode::SemiAuto;
		_recoil =			800.f;
		_fireRate =			1.f;
		_bulletLifespan =	0.3f;
		_bulletSpeed =		1200.f;
		_spread =			30.f;
		_bulletDamage =		10.f;
		_bulletAmount =		6;
		_dropDamage =		10.f;
	}
};