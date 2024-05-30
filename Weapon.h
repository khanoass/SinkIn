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
	sf::Vector2f _direction, _holdOffset, _tubeExit;
	float _speed, _friction, _angle, _rotSpeed, _rotFriction;
	int _shot;
	bool _shooting;
	std::vector<sf::Vector2f> _bounds;

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

	// Must be defined by subclass!
	sf::Vector2f	_bulletSize, _origin;
	int				_bulletAmount;
	float			_recoil, _fireRate, _bulletLifespan, _bulletSpeed, _spread, _bulletDamage,
					_dropSpeed, _dropRotationSpeed;
	Mode _mode;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	virtual void picked(const std::shared_ptr<Player>& player);

	void createBullet(const sf::Vector2f& mousePos);

public:
	enum Mode { Auto, SemiAuto, Burst3 };

	Weapon();

	Weapon(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Vector2f& origin, const sf::Vector2f& holdOffset, const sf::Vector2f& tubeExit, sf::Texture* textureGround, sf::Texture* textureHold, sf::Texture* textureMuzzle, int startAmmo);

	// Interaction
	virtual void pick(const std::shared_ptr<Player>& player) override;
	void shoot(const sf::Vector2f& dir);
	void setShooting(bool shooting);
	void drop(const sf::Vector2f& mousePos);

	void setActive(bool active);
	void setBounds(const sf::Vector2f& boundsX, const sf::Vector2f& boundsY);
	void clearBullets();

	Mode mode() const;
	int ammo() const;
	bool shooting() const;
	bool dropping() const;

	void update(float dt, const sf::Vector2f& mousePos);
	void updateDrop(float dt, const sf::Vector2f& mousePos);
};