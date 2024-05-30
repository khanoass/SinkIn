#include "Player.h"
#include "Map.h"

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(!_hitVis)
		target.draw(_sprite, states);
	if (_weapon != nullptr)
		target.draw(*_weapon, states);
#ifdef DEBUG
	if (_moving) target.draw(_line, states);
	target.draw(_reachShape, states);
	target.draw(_hitbox, states);
#endif
}

void Player::startMoving(const sf::Vector2f& target)
{
	_moving = true;
	_movTarget = target;

#ifdef DEBUG
	_line[0].position = _position;
	_line[1].position = _movTarget;
#endif

	_direction = vm::normalise((_movTarget - _position));
}

bool Player::reachedTarget()
{
	return vm::dist(_position, _movTarget) < 10.f;
}

void Player::shoot(const sf::Event& event)
{
	if (_weapon == nullptr) return;
	sf::Vector2f point = { (float)event.mouseButton.x, (float)event.mouseButton.y };
	sf::Vector2f final = finalCursorPosition(point);
	_moving = false;
	_weapon->shoot(final);
}

void Player::die()
{
	Logger::log({ "GAME OVER" });
}

Player::Player(ResManager* res)
	: _map(nullptr), _room(nullptr), _weapon(nullptr)
{
	_position = { 0, 0 };
	_boosts = 0;

	_size = { 60, 60 };

	_sprite.setOrigin({ _size.x / 2, _size.y / 2 });
	_sprite.setPosition(_position);
	_sprite.setTexture(res->textures.player);
	_sprite.setScale(_scale);

	_range = 35.f;
	_sprite.setPosition(_position);

#ifdef DEBUG
	_hitbox.setFillColor(sf::Color::Transparent);
	_hitbox.setOutlineColor(sf::Color::Red);
	_hitbox.setRadius(_range);
	_hitbox.setOrigin(_range, _range);
	_hitbox.setOutlineThickness(2.f);

	_reachShape.setRadius(300.f);
	_reachShape.setFillColor(sf::Color::Transparent);
	_reachShape.setOutlineColor(sf::Color(0, 0, 255, 255));
	_reachShape.setOutlineThickness(1.f);
	_reachShape.setPointCount(1000);
	_reachShape.setOrigin(300.f, 300.f);

	_line.setPrimitiveType(sf::Lines);
	_line.append(sf::Vertex(_position, sf::Color::Red));
	_line.append(sf::Vertex(_position, sf::Color::Red));
#endif
}

void Player::setMap(Map* map)
{
	_map = map;
	_room = map->currentRoom();
	_position = _room->center();
	_sprite.setPosition(_position);
	
#ifdef DEBUG
	_reachShape.setPosition(_position);
#endif
}

void Player::boost()
{
	_boosts++;
	float s = _baseSpeed + ((_boostsFactor * std::log10f(float(1 + _boosts))) * _baseSpeed);
	Logger::log({ "Player boosted to ", std::to_string(_boosts), ", speed: ", std::to_string(s) });

	// Darken in steps
	sf::Color newCol;
	if(_boosts > 0 && _boosts < 20)
		newCol = sf::Color(200, 200, 200);
	else if(_boosts < 30)
		newCol = sf::Color(150, 150, 150);
	else if (_boosts < 50)
		newCol = sf::Color(100, 100, 100);
	else if (_boosts < 75)
		newCol = sf::Color(100, 30, 30);

	_sprite.setColor(newCol);
}

bool Player::pickupWeapon(const std::shared_ptr<Weapon>& weapon)
{
	if (_weapon != nullptr) return false;
	_weapon = weapon;
	return true;
}

void Player::dropWeapon(const sf::Vector2f& mousePos)
{
	if (_weapon == nullptr) return;
	_weapon->drop(mousePos);
	_weapon = nullptr;
}

void Player::setKnockback(float knockback, const sf::Vector2f& direction)
{
	_knockback = knockback;
	_knockbackDirection = direction;
	_knockedback = true;
}

void Player::hit(float damage, const sf::Vector2f& direction)
{
	// Cooldown
	if (!_hit)
	{
		_hit = true;
		_hitVis = true;
		_hitClock.restart();
		_hitVisClock.restart();
		setKnockback(damage * _knockbackDamageFactor, direction);
		_hp -= damage;
		if (_hp <= 0)
			die();
	}
}

std::shared_ptr<Weapon> Player::activeWeapon()
{
	return _weapon;
}

sf::Vector2f Player::direction() const
{
	return _lookDirection;
}

sf::Vector2f Player::position() const
{
	return _position;
}

float Player::reach() const
{
	return _reach;
}

float Player::range() const
{
	return _range;
}

int Player::ammo() const
{
	if (_weapon == nullptr) return 0;
	return _weapon->ammo();
}

bool Player::pointInPlayer(const sf::Vector2f& point) const
{
	return _sprite.getGlobalBounds().contains(point);
}

sf::Vector2f Player::finalCursorPosition(const sf::Vector2f& mousePos) const
{
	// Check for reach
	sf::Vector2f player = _position;
	sf::Vector2f diff = mousePos - player;
	float dist = vm::norm(diff);
	sf::Vector2f finalPos = mousePos;
	if (dist > _reach)
		finalPos = (player + vm::normalise(diff) * _reach);
	return finalPos;
}

void Player::updateEvent(const sf::Event& event, float dt, const sf::Vector2f& mousePos)
{
	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::LShift)
	{
		if (_weapon != nullptr && !_weapon->dropping() && !pointInPlayer(mousePos))
			dropWeapon(mousePos);
	}

	// Start moving with click
	if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
	{
		sf::Vector2f point = { (float)event.mouseButton.x, (float)event.mouseButton.y };
		sf::Vector2f final = finalCursorPosition(point);

		if (_room->pointInRoom(final) && !pointInPlayer(final))
			startMoving(final);

		Direction dir = None;
		if (_room->pointInDoor(final, dir))
			startMoving(final);
	}

	// Weapon
	if (_weapon != nullptr)
	{
		// Shoot
		Weapon::Mode mode = _weapon->mode();

		switch (mode)
		{
		case Weapon::Auto:
		case Weapon::Burst3:
			if (!_weapon->shooting())
			{
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
					_weapon->setShooting(true);
			}
			else
			{
				if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right)
					_weapon->setShooting(false);
			}
			break;
		case Weapon::SemiAuto:
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
				shoot(event);
			break;	
		}
	}
}

void Player::update(float dt, const sf::Vector2f& mousePos)
{
	// Rotation follow mouse
	_lookDirection = vm::normalise((mousePos - _position));
	float angle = vm::angle(_lookDirection);
	_sprite.setRotation(angle);

	// Hit
	if (_hit)
	{
		if (_hitClock.getElapsedTime().asSeconds() > _hitCooldown)
		{
			_hit = false;
			_hitVis = false;
		}
		// Blinking effect when hit
		else if (_hitVisClock.getElapsedTime().asSeconds() > _hitVisSpeed)
		{
			_hitVisClock.restart();
			_hitVis = !_hitVis;
		}
	}

	// Movement update
	if (_moving || _knockedback)
	{
		if (_moving)
		{
			// a log(1 + x) function to calculate speed
			float s = _baseSpeed + ((_boostsFactor * std::log10f(float(1 + _boosts))) * _baseSpeed);
			_position.x += _direction.x * s * dt;
			_position.y += _direction.y * s * dt;
		}

		if (_knockedback)
		{
			_position.x += _knockbackDirection.x * _knockback * dt;
			_position.y += _knockbackDirection.y * _knockback * dt;
			_knockback *= _friction;

			if (_knockback < 1)
				_knockedback = false;
		}

#ifdef DEBUG
		_line[0].position = _position;
		_reachShape.setPosition(_position);
		_hitbox.setPosition(_position);
#endif

		std::vector<sf::Vector2f> border = {
			sf::Vector2f(_room->center().x - _room->size().x / 2, _room->center().x + _room->size().x / 2),
			sf::Vector2f(_room->center().y - _room->size().y / 2, _room->center().y + _room->size().y / 2)
		};

		// Out-of-bounds checking
		sf::Vector2f old(_position);
		if (_position.x - _size.x / 2 < border[0].x && !_room->pointInDoor(_position - sf::Vector2f(_size.x / 2, 0)))
			_position.x = border[0].x + _size.x / 2;
		if (_position.x + _size.x / 2 > border[0].y && !_room->pointInDoor(_position + sf::Vector2f(_size.x / 2, 0)))
			_position.x = border[0].y - _size.x / 2;
		if (_position.y - _size.y / 2 < border[1].x && !_room->pointInDoor(_position - sf::Vector2f(0, _size.y / 2)))
			_position.y = border[1].x + _size.y / 2;
		if (_position.y + _size.y / 2 > border[1].y && !_room->pointInDoor(_position + sf::Vector2f(0, _size.y / 2)))
			_position.y = border[1].y - _size.y / 2;
		if (_position != old) _moving = false;

		_sprite.setPosition(_position);

		if (reachedTarget()) _moving = false;
	}

	// Items
	std::shared_ptr<Item> item = _map->items()->closestReachableItem(_position, _range);
	if (item != nullptr)
		item->pick(shared_from_this());

	// Weapon
	if(_weapon != nullptr)
		_weapon->update(dt, mousePos);

	// Room exit
	Direction dir = None;
	if (_room->pointInDoor(_position, dir))
	{
		_map->exitRoom(dir);
		_room = _map->currentRoom();
		_moving = false;
		_position = _room->spawn(dir);
		_sprite.setPosition(_position);
		

#ifdef DEBUG
		_reachShape.setPosition(_position);
#endif

		Logger::log({ "Player entered: ", _room->name() });
	}
	else
	{
		if (_map->changedRoom())
			_map->resetChangedRoom();
	}
}
