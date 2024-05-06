#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>

class Animation
{
private:
	// Data
	sf::Clock _clock;
	sf::Vector2i _frameSize;
	sf::IntRect _frame;
	sf::Vector2i _numFrames, _iFrames;
	float _seconds = 0.f;
	bool _started = false;
	bool _loop = true;

public:
	Animation()
	{
	}

	void setSpritesheet(const sf::IntRect& first, const sf::Vector2i& numFrames, float seconds)
	{
		_frame = first;
		_frameSize = { first.width, first.height };
		_numFrames = numFrames;
		_seconds = seconds;
		_iFrames = { 0, 0 };
	}

	void start(bool loop = true)
	{
		_loop = loop;
		_clock.restart();
		_started = true;
	}

	void stop()
	{
		_started = false;
	}

	bool changed()
	{
		if (!_started) return false;
		if (_clock.getElapsedTime().asSeconds() < _seconds) return false;

		if (_iFrames.x < _numFrames.x - 1)
		{
			_frame.left += _frameSize.x;
			_iFrames.x++;
		}
		else if (_iFrames.y < _numFrames.y - 1)
		{
			_frame.left = 0;
			_frame.top += _frameSize.y;
			_iFrames.x = 0;
			_iFrames.y++;
		}
		else
		{
			if (!_loop)
			{
				_started = false;
				return true;
			}
			_frame.left = 0;
			_frame.top = 0;
			_iFrames.x = 0;
			_iFrames.y = 0;
		}

		_clock.restart();
		return true;
	}

	sf::IntRect frame() const
	{
		return _frame;
	}

	bool finished() const
	{
		return !_started;
	}
};