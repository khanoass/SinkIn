#pragma once

#include <SFML/Graphics/Shader.hpp>

#include "Levels.h"
#include "Camera.hpp"
#include "GUI.hpp"

class Game : public Entity
{
public:
	enum State
	{
		Play, Story, GameOver, Loading
	};

private:
	const float _viewMargin = 200.f;

	// Data
	sf::Vector2f _screenCenter;

	State _state;
	Levels _levels;
	GUI _gui;
	Camera _camera, _guiCamera;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		switch (_state)
		{
		case Game::Play:
			target.draw(_levels, states);
			target.draw(_gui, states);
			break;
		case Game::Story:
			target.draw(_levels, states);
			break;
		case Game::GameOver:
			break;
		case Game::Loading:
			break;
		}
	}

	std::shared_ptr<Map> map()
	{
		return _levels.map();
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

public:
	Game(const sf::Vector2f& center, ResManager* res) :
		_state(Play),
		_screenCenter(center),
		_levels({ "assets/maps/map1.png" }, { "assets/sequences/intro-en.txt" }, _screenCenter, res),
		_gui(_levels.map(), res),
		_camera({ (unsigned int)center.x * 2, (unsigned int)center.y * 2 }, 0.9f),
		_guiCamera({ (unsigned int)center.x * 2, (unsigned int)center.y * 2 })
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

	sf::Vector2f getPlayerScreenPosition()
	{
		return _levels.player()->absolutePosition();
	}

	void setVignettePosition(const sf::Vector2f& pos)
	{
		_levels.setVignettePosition(pos);
	}

	sf::View& view()
	{
		return _camera.view();
	}

	sf::View& guiView()
	{
		return _guiCamera.view();
	}

	void updateEvent(const sf::Event& event, float dt, const sf::Vector2f& mousePos)
	{
		_levels.updateEvent(event, dt, mousePos);
		if (_levels.sequence())
			_state = State::Story;
		else
			_state = State::Play;
	}

	void update(float dt, const sf::Vector2f& mousePos, int fps)
	{
		if (_state == State::Play)
		{
			// Camera
			auto size = map()->currentRoom()->size();
			sf::Vector2f sizeplus = { size.x + _viewMargin, size.y + _viewMargin };
			_camera.updatePlayerSmooth(dt, getPlayerScreenPosition(), map()->currentRoom()->screenCenter(), sizeplus, map()->changedRoom());
		
			// GUI
			_gui.setFPS((int)fps);
			if (playerHasWeapon())	_gui.setAmmo(getPlayerAmmo());
			else					_gui.setAmmo(-1);
			_gui.setBoostTime(getPlayerBoostTime(), getPlayerMaxBoostTime());
			_gui.setHP(getPlayerHP(), getPlayerMaxHP());
			_gui.update(dt, mousePos);
		}

		_levels.update(dt, mousePos);
	}
};