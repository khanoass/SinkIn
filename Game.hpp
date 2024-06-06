#pragma once

#include <SFML/Graphics/Shader.hpp>

#include "Levels.h"

class Game : public LiveEntity
{
public:
	enum State
	{
		Play, Story, GameOver, Loading
	};

private:
	// Data
	sf::Vector2f _screenCenter;

	State _state;
	Levels _levels;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (_state == Play || _state == Story)
			target.draw(_levels, states);
	}

public:
	Game(const sf::Vector2f& center, ResManager* res) :
		_state(Play),
		_screenCenter(center),
		_levels({ "assets/maps/map1.png" }, { "assets/sequences/intro-en.txt" }, _screenCenter, res)
	{
	}

	State state() const
	{
		return _state;
	}

	void setState(State state)
	{
		_state = state;
	}

	std::shared_ptr<Map> map()
	{
		return _levels.map();
	}

	sf::Vector2f getPlayerScreenPosition()
	{
		return _levels.player()->absolutePosition();
	}

	int getPlayerAmmo()
	{
		return _levels.player()->ammo();
	}

	bool playerHasWeapon()
	{
		return _levels.player()->activeWeapon() != nullptr;
	}

	float getPlayerBoostTime()
	{
		return _levels.player()->boostTime();
	}

	float getPlayerMaxBoostTime()
	{
		return _levels.player()->maxBoostTime();
	}

	int getPlayerHP()
	{
		return _levels.player()->hp();
	}

	int getPlayerMaxHP()
	{
		return _levels.player()->maxHp();
	}

	void setVignettePosition(const sf::Vector2f& pos)
	{
		_levels.setVignettePosition(pos);
	}

	void updateEvent(const sf::Event& event, float dt, const sf::Vector2f& mousePos)
	{
		_levels.updateEvent(event, dt, mousePos);
		if (_levels.sequence())
			_state = Game::Story;
		else
			_state = Game::Play;
	}

	virtual void update(float dt, const sf::Vector2f& mousePos) override
	{
		_levels.update(dt, mousePos);
	}
};