#pragma once

#include "Level.hpp"

class Levels : public LiveEntity
{
private:
	// Data
	sf::Vector2f _screenCenter;
	std::vector<Level> _levels;
	int _i;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(_levels[_i], states);
	}

public:
	Levels(const std::vector<std::string>& filnames, const sf::Vector2f& center, ResManager* res)
	{
		_screenCenter = center;

		for (const auto& f : filnames)
			_levels.emplace_back(f, center, res);

		_i = 0;
		_levels[_i].start();
	}

	void inc()
	{
		_i++;
	}

	std::shared_ptr<Player> player()
	{
		return _levels[_i].player();
	}

	std::shared_ptr<Map> map()
	{
		return _levels[_i].map();
	}

	void updateEvent(const sf::Event& event, float dt, const sf::Vector2f& mousePos)
	{
		_levels[_i].updateEvent(event, dt, mousePos);
	}

	void setVignettePosition(const sf::Vector2f& pos)
	{
		map()->getTexShaderPtr()->setUniform("playerPos", sf::Glsl::Vec2(pos));
	}

	virtual void update(float dt, const sf::Vector2f& mousePos)
	{
		_levels[_i].update(dt, mousePos);

		map()->getTexShaderPtr()->setUniform("screenSize", sf::Glsl::Vec2({ _screenCenter.x * 2, _screenCenter.y * 2 }));

		if(player()->boosted())	map()->getTexShaderPtr()->setUniform("radius", 0.85f);
		else					map()->getTexShaderPtr()->setUniform("radius", 0.5f);
	}
};