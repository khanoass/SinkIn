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
		Start, Play, Story, PausePlay, PauseStory, GameOver
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
		case Game::Start:
			target.draw(_start, states);
			break;
		case Game::Play:
		case Game::Story:
		case Game::PausePlay:
		case Game::PauseStory:
			target.draw(_levels, states);		
			break;
		case Game::GameOver:
			break;
		}
	}

public:
	Game(const sf::Vector2f& center, ResManager* res) :
		_state(Start),
		_screenCenter(center),
		_levels({ "assets/maps/tutorial.png" }, { "assets/sequences/intro-en.txt" }, _screenCenter, res),
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

	PauseScreen& pauseScreen()
	{
		return _pause;
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
		if (_state == Play || _state == Story)
		{
			_levels.updateEvent(event, dt, mousePos);
			if (_levels.sequence())	_state = State::Story;
			else					_state = State::Play;
		}

		switch (_state)
		{
		case Game::Start:
			_start.updateEvent(event);
			break;

		case Game::Play:
			// Pause from play
			if (event.type == event.KeyReleased && event.key.code == sf::Keyboard::Escape)
				_state = State::PausePlay;
			break;
		case Game::Story:
			if (event.type == event.KeyReleased && event.key.code == sf::Keyboard::Escape)
				_state = State::PauseStory;
			break;
		case Game::PausePlay:
		case Game::PauseStory:
			_pause.updateEvent(event);
			break;
		case Game::GameOver:
			break;
		}
	}

	void update(float dt, const sf::Vector2f& mousePos, int fps)
	{
		switch (_state)
		{
		case State::Start:
			_start.update(mousePos);
			if (_start.started())
			{
				_state = (_levels.sequence()) ? State::Story : State::Play;
			}
			if (_start.exited())
				_exit = true;
			break;

		case State::Play:
		{
			// Camera
			auto size = _levels.map()->currentRoom()->size();
			sf::Vector2f sizeplus = { size.x + _viewMargin, size.y + _viewMargin };
			_camera.updatePlayerSmooth(dt, getPlayerScreenPosition(), _levels.map()->currentRoom()->screenCenter(), sizeplus, _levels.map()->changedRoom());

			// GUI
			_gui.setFPS((int)fps);
			if (_levels.player()->activeWeapon() != nullptr)
				_gui.setAmmo(_levels.player()->ammo());
			else
				_gui.setAmmo(-1);
			_gui.setBoostTime(_levels.player()->boostTime(), _levels.player()->maxBoostTime());
			_gui.setHP(_levels.player()->hp(), _levels.player()->maxHp());
			_gui.setKeys(_levels.player()->keys(), _levels.map()->keys());
			_gui.update(dt, mousePos);

			// Game
			_levels.update(dt, mousePos);
			break;
		}

		case State::Story:
			_levels.update(dt, mousePos);
			break;

		case State::PausePlay:
			_pause.update(mousePos);
			if (_pause.resumed())
				_state = State::Play;
			if (_pause.exited())
				_state = State::Start;
			break;

		case State::PauseStory:
			_pause.update(mousePos);
			if (_pause.resumed())
				_state = State::Story;
			if (_pause.exited())
				_state = State::Start;
			break;

		case State::GameOver:
			break;
		}
	}
};