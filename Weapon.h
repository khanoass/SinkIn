#pragma once

#include <SFML/System/Clock.hpp>

#include "Item.hpp"
#include "Random.hpp"
#include "ResManager.hpp"
#include "Ephemereal.hpp"

struct Bullet
{
	sf::Vector2f position, size, direction, origin;
	float timeleft, speed, damage;

	bool operator== (const Bullet& other)
	{
		return
			position == other.position &&
			size == other.size &&
			direction == other.direction &&
			timeleft == other.timeleft &&
			speed == other.speed &&
			origin == other.origin &&
			damage == other.damage;
	}
};

class Weapon : public Item, public std::enable_shared_from_this<Weapon>
{
public:
	enum Mode;

private:
	// Data
	std::vector<Bullet> _bullets;
	bool _active;
	sf::Vector2f _direction;
	float _speed, _friction;
	int _shot;
	bool _shooting;

	// Cosmetic
	sf::Sprite _pickedUp;
	std::vector<sf::Vertex> _bulletArray;

#ifdef DEBUG
	sf::CircleShape _gunorigin, _gunExit;
#endif

	sf::Vector2f tubeExit(const sf::Vector2f& mousePos) const;
	sf::Vector2f gunCenter() const;
	sf::Vector2f gunDir(const sf::Vector2f& mousePos) const;

	// Flash animation
	Ephemereal _eph;
	sf::Texture* _ephSheet;

	float _cooldown = 0;
	bool _dropping = false;

protected:
	std::shared_ptr<Player> _player;
	int _ammo;

	// Must be defined!
	sf::Vector2f _holdOffset, _tubeExit, _bulletSize, _origin;
	int _magSize;
	float _recoil, _fireRate, _bulletLifespan, _bulletSpeed, _spread, _reloadTime, _bulletDamage;
	Mode _mode;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	virtual void picked(const std::shared_ptr<Player>& player);

public:
	enum Mode
	{
		Auto, SemiAuto, Burst3
	};

	Weapon();

	Weapon(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Vector2f& origin, sf::Texture* textureGround, sf::Texture* textureHold, sf::Texture* textureMuzzle);

	void shoot(const sf::Vector2f& dir);

	void reload();

	Mode mode() const;

	int ammo() const;

	bool shooting() const;
	void setShooting(bool shooting);
	int shot() const;
	void resetShot();

	void setActive(bool active);

	void drop(const sf::Vector2f& mousePos);
	bool dropping() const;

	void update(float dt, const sf::Vector2f& mousePos);
};