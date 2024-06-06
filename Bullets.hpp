#pragma once

#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>

struct Bullet
{
	sf::Vector2f position, size, direction, origin;
	float timeleft, speed, damage;
	bool dead = false;

	bool operator== (const Bullet& other)
	{
		return
			position == other.position &&
			size == other.size &&
			direction == other.direction &&
			timeleft == other.timeleft &&
			speed == other.speed &&
			origin == other.origin &&
			damage == other.damage &&
			dead == other.dead;
	}
};

class Bullets : public sf::Drawable
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (_bulletArray.size() > 0)
			target.draw(_bulletArray.data(), _bulletArray.size(), sf::Quads, states);
	}

	std::vector<Bullet> _bullets;
	std::vector<sf::Vertex> _bulletArray;
	std::vector<sf::Vector2f> _bounds;

public:
	Bullets()
	{
	}

	void setBounds(const sf::Vector2f& boundsX, const sf::Vector2f& boundsY)
	{
		_bounds.clear();
		_bounds.push_back(boundsX);
		_bounds.push_back(boundsY);
	}

	std::vector<sf::Vector2f>& getBounds()
	{
		return _bounds;
	}

	void clearBullets()
	{
		_bullets.clear();
		_bulletArray.clear();
	}

	std::vector<Bullet>& getBullets()
	{
		return _bullets;
	}

	void addBullet(const Bullet& bullet)
	{
		_bulletArray.insert(_bulletArray.end(), {
			sf::Vertex(bullet.position, sf::Color::Transparent),
			sf::Vertex({ bullet.position.x + bullet.size.x, bullet.position.y }, sf::Color::Yellow),
			sf::Vertex({ bullet.position.x + bullet.size.x, bullet.position.y + bullet.size.y }, sf::Color::Transparent),
			sf::Vertex({ bullet.position.x, bullet.position.y + bullet.size.y }, sf::Color::Yellow)
			});

		_bullets.push_back(bullet);
	}

	void update(float dt, const sf::Vector2f& mousePos)
	{
		// Remove timedout & dead bullets
		for (int i = 0; i < _bullets.size(); i++)
		{
			_bullets[i].timeleft -= dt;
			if (_bullets[i].timeleft <= 0 || _bullets[i].dead)
			{
				_bullets.erase(_bullets.begin() + i);
				int vi = i * 4;
				_bulletArray.erase(_bulletArray.begin() + vi, _bulletArray.begin() + vi + 4);
			}
		}

		// Update bullet position & vertices
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
			_bulletArray[vi + 2].position = { b.position.x + b.size.x, b.position.y + b.size.y };
			_bulletArray[vi + 3].position = { b.position.x, b.position.y + b.size.y };
		}
	}
};