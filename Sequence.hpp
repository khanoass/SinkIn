#pragma once

#include <fstream>
#include <sstream>
#include <SFML/Window/Event.hpp>
#include "Board.hpp"

class Sequence : public sf::Drawable
{
private:
	// Data
	const std::string _continue = "[Left Click] to continue...";
	const unsigned int _charSize = 24u;
	const sf::Vector2f _marginRightBottom = { 50.f, 50.f };

	ResManager* _res;
	bool _finished = false;

	sf::Vector2f _center;
	std::vector<Board> _boards;
	int _i;

	sf::Text _continueText;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (!_finished)
		{
			target.draw(_boards[_i], states);
			if(_boards[_i].finished())
				target.draw(_continueText, states);
		}
	}

	bool loadFromFile(const std::string& filename)
	{
		std::ifstream is(filename);
		std::string line;
		if (!is.is_open()) return false;

		while (std::getline(is, line))
		{
			std::istringstream ss(line);
			std::vector<std::string> lines;

			for (std::string each; std::getline(ss, each, '|'); lines.push_back(each));

			if (lines[0][0] == '#')
			{
				lines[0].erase(0, 1);
				// Show boost
				_boards.emplace_back(_center, lines, _res, true);
			}
			else
			{
				// Text display
				_boards.emplace_back(_center, lines, _res);
			}
		}

		return true;
	}

public:
	Sequence(const std::string& filename, const sf::Vector2f& center, ResManager* res)
	{
		_center = center;
		_res = res;

		if (!loadFromFile(filename)) return;
		_i = 0;

		_continueText.setCharacterSize(_charSize);
		_continueText.setString(_continue);
		_continueText.setFont(res->fonts.font);
		auto bounds = _continueText.getGlobalBounds();
		_continueText.setPosition(_center.x * 2 - bounds.width - _marginRightBottom.x, _center.y * 2 - bounds.height - _marginRightBottom.y);
	}

	void start()
	{
		_boards[_i].start();
	}

	void updateEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
		{
			if (_boards[_i].finished())
			{
				_i++;
				if (_i >= _boards.size())
					_finished = true;
				else
					_boards[_i].start();
			}
			else
				_boards[_i].skip();
		}
	}

	bool finished() const
	{
		return _finished;
	}

	void update(float dt, const sf::Vector2f& mousePos)
	{
		_boards[_i].update(dt, mousePos);
	}
};