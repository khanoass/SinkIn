#pragma once

#include "Weapon.h"

class Pistol : public Weapon
{
public:
	Pistol(const sf::Vector2f& position, ResManager* res)
		: Weapon(position, { 50, 50 }, { 25, 25 }, & res->textures.glock_ground, &res->textures.glock_hold, &res->textures.attack)
	{
		_holdOffset = { -40, 30 };
		_tubeExit = { 0, -30 };
		_bulletSize = { 5, 5 };
		_magSize = { 20 };
		_recoil = { 50 };
		_fireRate = { 0 };
		_mode = Mode::SemiAuto;
		_bulletLifespan = { 1 };
		_bulletSpeed = { 200 };
		_spread = { 0 };
		_reloadTime = { 4 };
		_bulletDamage = { 10 };
	}
};