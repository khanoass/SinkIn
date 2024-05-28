#pragma once

#include "Weapon.h"

class Pistol : public Weapon
{
public:
	Pistol(const sf::Vector2f& position, ResManager* res)
		: Weapon(position, { 50, 50 }, { 25, 25 }, &res->textures.glock_ground, &res->textures.glock_hold, &res->textures.muzzle, 2000)
	{
		_holdOffset = { -40, 30 };
		_tubeExit = { 0, -30 };
		_bulletSize = { 5, 5 };
		_recoil = { 500 };
		_fireRate = { 0.05 };
		_mode = Mode::Auto;
		_bulletLifespan = { 0.3 };
		_bulletSpeed = { 1200 };
		_spread = { 10 };
		//_reloadTime = { 4 };
		//_bulletDamage = { 10 };
		//_magSize = { 0 };

		_dropSpeed = 2000;
		_dropRotationSpeed = 2000;
	}
};