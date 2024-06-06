#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "ResManager.hpp"

class Board : public sf::Drawable
{
private:
	const unsigned int _charSize = 28u;
	const sf::Vector2f _spacing = { 30.f, 80.f };
	const float _Yoffset = 300.f;
	const float _timeShow = 0.05f;

	// Data
	std::string _textString;
	sf::Vector2f _center;
	bool _started = false;
	bool _finished = false;
	bool _boost = false;

	std::vector<sf::Text> _chars;
	std::vector<bool> _showed;
	int _index;
	sf::Clock _clock;

	sf::Sprite _fakeBoost;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (int i = 0; i < _chars.size(); i++)
		{
			if(_showed[i])
				target.draw(_chars[i], states);
			if (_boost)
				target.draw(_fakeBoost);
		}
	}

public:
	Board(const sf::Vector2f& center, const std::vector<std::string>& lines, ResManager* res, bool boost = false)
	{
		_center = center;
		_index = 0;
		
		// TODO: Generalize
		_boost = boost;
		_fakeBoost.setTexture(res->textures.boost);
		_fakeBoost.setOrigin({ 16, 16 });
		_fakeBoost.setScale(5.f, 5.f);
		_fakeBoost.setColor(sf::Color(255, 255, 255, 100));
		_fakeBoost.setPosition({ _center.x, _center.y + 150.f });

		for (int i = 0; i < lines.size(); i++)
		{
			auto sizeX = lines[i].size() * _spacing.x;

			sf::Vector2f position = {
				_center.x - sizeX / 2,
				_Yoffset + i * _spacing.y
			};

			for (int j = 0; j < lines[i].size(); j++)
			{
				sf::Text t;
				t.setCharacterSize(_charSize);
				t.setFont(res->fonts.retro);
				t.setPosition(position.x + j * _spacing.x, position.y);
				t.setString(lines[i][j]);
				_chars.push_back(t);
				_showed.push_back(false);
			}
		}
	}

	void start()
	{
		_started = true;
		_clock.restart();
	}

	void skip()
	{
		_started = false;
		_finished = true;
		for (int i = 0; i < _showed.size(); i++)
			_showed[i] = true;
	}

	bool finished() const
	{
		return _finished;
	}

	void update(float dt, const sf::Vector2f& mousePos)
	{
		if (!_started) return;

		if (_clock.getElapsedTime().asSeconds() > _timeShow)
		{
			_showed[_index] = true;
			_index++;
			if (_index >= _showed.size())
			{
				_finished = true;
				_started = false;
				return;
			}
			_clock.restart();
		}
	}
};