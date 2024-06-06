#pragma once

#include <SFML/Graphics/Shader.hpp>

#include "Levels.h"
#include "Camera.hpp"
#include "GUI.hpp"
#include "StartScreen.hpp"
#include "PauseScreen.hpp"

class Game : public sf::Drawable
{
public:
	enum State
	{
		Play, Story, Start, GameOver, Pause
	};

private:
	const float _viewMargin = 200.f;

	// Data
	sf::Vector2f _screenCenter;

	State _state;
	Levels _levels;
	GUI _gui;
	Camera _camera, _guiCamera;
	StartScreen _start;
	PauseScreen _pause;
	bool _exit = false;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		switch (_state)
		{
		case Game::Play:
			target.draw(_levels, states);		
			break;
		case Game::Story:
			target.draw(_levels, states);
			break;
		case Game::Start:
			target.draw(_start, states);
			break;
		case Game::GameOver:
			break;
		case Game::Pause:
			target.draw(_pause, states);
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

	float getPlayerHP()
	{
		return _levels.player()->hp();
	}

	float getPlayerMaxHP()
	{
		return _levels.player()->maxHp();
	}

public:
	Game(const sf::Vector2f& center, ResManager* res) :
		_state(Start),
		_screenCenter(center),
		_levels({ "assets/maps/map1.png" }, { "assets/sequences/intro-en.txt" }, _screenCenter, res),
		_gui(_levels.map(), res),
		_camera({ (unsigned int)center.x * 2, (unsigned int)center.y * 2 }, 0.9f),
		_guiCamera({ (unsigned int)center.x * 2, (unsigned int)center.y * 2 }),
		_start(center, res),
		_pause(center, res)
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

	GUI& gui()
	{
		return _gui;
	}

	bool exit() const
	{
		return _exit;
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
		if (_state == State::Start)
		{
			_start.updateEvent(event);
		}
		else if (_state == State::Story || _state == State::Play)
		{
			// Pause
			if (event.type == event.KeyReleased && event.key.code == sf::Keyboard::Escape)
			{
				_state = State::Pause;
				return;
			}

			_levels.updateEvent(event, dt, mousePos);
			if (_levels.sequence())	_state = State::Story;
			else					_state = State::Play;
		}
		else if (_state == State::Pause)
		{
			_pause.updateEvent(event);
		}
	}

	void update(float dt, const sf::Vector2f& mousePos, int fps)
	{
		switch (_state)
		{
		case State::Start:
			_start.update(mousePos);
			if (_start.started())
				_state = State::Story;
			if (_start.exited())
			{
				_exit = true;
				return;
			}
			break;
		case State::Pause:
			_pause.update(mousePos);
			if (_pause.resumed())
				_state = State::Play;
			if (_pause.exited())
				_state = State::Start;
			break;
		case State::Play:
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

			// Game
			_levels.update(dt, mousePos);
			break;
		}
		case State::Story:
			_levels.update(dt, mousePos);
			break;
		}
	}
};