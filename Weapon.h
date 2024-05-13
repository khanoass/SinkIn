#pragma once

#include <SFML/System/Clock.hpp>

#include "Item.hpp"
#include "Random.hpp"
#include "ResManager.hpp"
#include "Ephemereal.hpp"

struct Bullet
{
	sf::Vector2f position, size, direction, origin;
	float lifespan, speed, damage;
	sf::Clock clock;

	bool operator== (const Bullet& other)
	{
		return
			position == other.position &&
			size == other.size &&
			direction == other.direction &&
			lifespan == other.lifespan &&
			speed == other.speed &&
			origin == other.origin &&
			damage == other.damage &&
			clock.getElapsedTime() == other.clock.getElapsedTime();
	}
};

class Weapon : public Item
{
public:
	enum Mode;

private:
	// Data
	std::vector<Bullet> _bullets;
	bool _active;  

	// Cosmetic
	sf::Sprite _pickedUp;
	std::vector<sf::Vertex> _bulletArray;

#ifdef DEBUG
	sf::CircleShape _gunorigin, _gunExit;
#endif

	sf::Vector2f tubeExit(const sf::Vector2f& mousePos) const;
	sf::Vector2f gunCenter() const;
	float gunDir(const sf::Vector2f& mousePos) const;

	// Flash animation
	Ephemereal _eph;
	sf::Texture* _ephSheet;

protected:
	// TODO: optimise (bullets vector useless?)

	Player* _player;
	int _ammo;

	// Must be defined!
	sf::Vector2f _holdOffset, _tubeExit, _bulletSize, _origin;
	int _magSize;
	float _recoil, _fireRate, _bulletLifespan, _bulletSpeed, _spread, _reloadTime, _bulletDamage;
	Mode _mode;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	virtual void picked(Player* player);

public:
	enum Mode
	{
		Auto, SemiAuto, Burst3
	};

	Weapon(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Vector2f& origin, sf::Texture* textureGround, sf::Texture* textureHold, sf::Texture* textureMuzzle);

	void shoot(const sf::Vector2f& dir);

	void reload();

	void setActive(bool active);

	void update(float dt, const sf::Vector2f& mousePos);
};