#include "Weapon.h"
#include "Player.h"

enum Weapon::Mode;

sf::Vector2f Weapon::tubeExit(const sf::Vector2f& mousePos) const
{
	auto newexit = vm::rotateVector(_tubeExit, vm::angle(gunDir(mousePos)));
	return newexit + gunCenter();
}

sf::Vector2f Weapon::gunCenter() const
{
	float angle = vm::angle(_player->direction());
	auto newhold = vm::rotateVector(_holdOffset, angle);
	auto finalPos = _player->position() + newhold;
	return finalPos;
}

sf::Vector2f Weapon::gunDir(const sf::Vector2f& mousePos) const
{
	return vm::normalise(gunCenter() - mousePos);
}

void Weapon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Item::draw(target, states);
	if (_active)
	{
		target.draw(_eph, states);
		target.draw(_pickedUp, states);
		if (_bulletArray.size() > 0)
			target.draw(_bulletArray.data(), _bulletArray.size(), sf::Quads, states);

#ifdef DEBUG
		target.draw(_gunorigin, states);
		target.draw(_gunExit, states);
#endif

	}
}

void Weapon::picked(const std::shared_ptr<Player>& player)
{
	if (_alive)
	{
		if (player->pickupWeapon(shared_from_this()))
		{
			_player = player;
			Logger::log({ "Weapon picked" });
			_active = true;
			_alive = false;
		}
	}
}

void Weapon::createBullet(const sf::Vector2f& mousePos)
{
	Bullet bullet;
	bullet.position = tubeExit(mousePos);
	bullet.size = _bulletSize;
	bullet.timeleft = _bulletLifespan;
	bullet.speed = _bulletSpeed;
	bullet.damage = _bulletDamage;

	sf::Vector2f direction = vm::normalise(tubeExit(mousePos) - gunCenter());
	float angle = Random::fRand(-_spread / 2, _spread / 2);
	direction = vm::rotateVector(direction, angle);

	bullet.direction = direction;

	_bulletArray.insert(_bulletArray.end(), {
		sf::Vertex(bullet.position, sf::Color::Transparent),
		sf::Vertex({ bullet.position.x + bullet.size.x, bullet.position.y }, sf::Color::Yellow),
		sf::Vertex({ bullet.position.x + bullet.size.x, bullet.position.y + bullet.size.y }, sf::Color::Transparent),
		sf::Vertex({ bullet.position.x, bullet.position.y + bullet.size.y }, sf::Color::Yellow)
		});

	_bullets.push_back(bullet);

	// Muzzle flash
	sf::Vector2f pos = tubeExit(mousePos);
	_eph.spawn(pos, { 64, 64 }, _ephSheet, { 5, 2 }, (float)0.05, angle);
}

Weapon::Weapon() : Item({0, 0}, {0, 0}, nullptr)
{
}

Weapon::Weapon(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Vector2f& origin, const sf::Vector2f& holdOffset, const sf::Vector2f& tubeExit, sf::Texture* textureGround, sf::Texture* textureHold, sf::Texture* textureMuzzle, int startAmmo)
	: Item(position, size, textureGround)
{
	_active = false;
	_origin = origin;
	_holdOffset = holdOffset;
	_tubeExit = tubeExit;

	_ephSheet = textureMuzzle;

	_pickedUp.setOrigin(_origin);
	_pickedUp.setPosition(_position);
	_pickedUp.setScale({ 2.f, 2.f });
	_pickedUp.setTexture(*textureHold);

	_ammo = startAmmo;

	_shot = 0;
	_shooting = false;

	_speed = 0;
	_direction = { 0, 0 };
	_friction = 0;

#ifdef DEBUG
	_gunorigin.setFillColor(sf::Color::Blue);
	_gunorigin.setRadius(5.f);
	_gunorigin.setOrigin(5.f, 5.f);

	_gunExit.setFillColor(sf::Color::Magenta);
	_gunExit.setRadius(5.f);
	_gunExit.setOrigin(5.f, 5.f);
#endif
}

void Weapon::pick(const std::shared_ptr<Player>& player)
{
	picked(player);
}

void Weapon::shoot(const sf::Vector2f& mousePos)
{
	if (_player->pointInPlayer(mousePos))
		return;

	if (_ammo == 0)
	{
		Logger::log({ "no ammo..." });
		return;
	}

	// Cooldown
	if (_cooldown > 0)
	{
		Logger::log({ "cooldown..." });
		return;
	}
	_cooldown = _fireRate;

	Logger::log({ "pew !" });

	_ammo--;
	_shot++;

	for (int i = 0; i < _bulletAmount; i++)
		createBullet(mousePos);

	// Knockback
	_player->setKnockback(_recoil, gunDir(mousePos));
}

void Weapon::setBounds(const sf::Vector2f& boundsX, const sf::Vector2f& boundsY)
{
	_bounds.push_back(boundsX);
	_bounds.push_back(boundsY);
}

void Weapon::clearBullets()
{
	_bullets.clear();
	_bulletArray.clear();
}

Weapon::Mode Weapon::mode() const
{
	return _mode;
}

int Weapon::ammo() const
{
	return _ammo;
}

bool Weapon::shooting() const
{
	return _shooting;
}

void Weapon::setShooting(bool shooting)
{
	_shooting = shooting;
	if (shooting)
		_shot = 0;
}

void Weapon::setActive(bool active)
{
	_active = active;
}

void Weapon::drop(const sf::Vector2f& mousePos)
{
	_alive = true;
	_active = false;
	_direction = -gunDir(mousePos);
	_speed = _dropSpeed;
	_rotSpeed = _dropRotationSpeed;
	_friction = 0.95;
	_rotFriction = 0.98;
	_angle = 0;
	_position = tubeExit(mousePos) - _direction * 10.f;
	setGroundSettings(_position, _angle);
	_dropping = true;
	Logger::log({ "Weapon dropped" });
}

bool Weapon::dropping() const
{
	return _dropping;
}

void Weapon::update(float dt, const sf::Vector2f& mousePos)
{
	// Cooldown update
	if (_cooldown > 0)
		_cooldown -= dt;

	// Auto shooting update
	if (_shooting)
	{
		// Auto
		if (_mode == Mode::Auto)
			shoot(mousePos);

		// Burst3
		if (_mode == Mode::Burst3)
		{
			if (_shot >= 3)
			{
				_shot = 0;
				_shooting = false;
			}
			else
				shoot(mousePos);
		}
	}

	// Remove timedout bullets
	for (int i = 0; i < _bullets.size(); i++)
	{
		_bullets[i].timeleft -= dt;
		if (_bullets[i].timeleft <= 0)
		{
			_bullets.erase(_bullets.begin() + i);
			int vi = i * 4;
			_bulletArray.erase(_bulletArray.begin() + vi, _bulletArray.begin() + vi + 4);
		}
	}

	// Update position & vertices
	for (int i = 0; i < _bullets.size(); i++)
	{
		auto& b = _bullets[i];
		b.position.x += b.direction.x * b.speed * dt;
		b.position.y += b.direction.y * b.speed * dt;

		// Collisions with walls
		if (b.position.x < _bounds[0].x || b.position.x > _bounds[0].y)	b.direction.x *= -1.f;
		if (b.position.y < _bounds[1].x || b.position.y > _bounds[1].y)	b.direction.y *= -1.f;

		int vi = i * 4;

		_bulletArray[vi].position = b.position;
		_bulletArray[vi + 1].position = { b.position.x + b.size.x, b.position.y };
		_bulletArray[vi+2].position = { b.position.x + b.size.x, b.position.y + b.size.y };
		_bulletArray[vi+3].position = { b.position.x, b.position.y + b.size.y };
	}

	_eph.update(dt, mousePos);

	// Hold
	auto pos = gunCenter();

	_pickedUp.setPosition(pos);
	_pickedUp.setRotation(vm::angle(gunDir(mousePos)));

#ifdef DEBUG
	_gunorigin.setPosition(pos);
	_gunExit.setPosition(tubeExit(mousePos));
#endif
}

void Weapon::updateDrop(float dt, const sf::Vector2f& mousePos)
{
	if (_dropping && vm::norm(_direction) > 0 && _speed > 0)
	{
		if (_speed < 1)
		{
			_direction = { 0, 0 };
			_speed = 0;
			_dropping = false;
		}
		else
		{
			_position.x += _direction.x * _speed * dt;
			_position.y += _direction.y * _speed * dt;
			_angle += _rotSpeed * dt;
			_speed *= _friction;
			_rotSpeed *= _rotFriction;

			// Collisions with walls
			if (_position.x < _bounds[0].x || _position.x > _bounds[0].y)	_direction.x *= -1.f;
			if (_position.y < _bounds[1].x || _position.y > _bounds[1].y)	_direction.y *= -1.f;
		}

		setGroundSettings(_position, _angle);
	}
}
