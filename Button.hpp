#pragma once

#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "ResManager.hpp"

class Button : public sf::Drawable
{
private:
	enum State
	{
		Default, Hover, Clicked
	};

	// Colors
	const sf::Color _bg = { 0, 0, 0 };
	const sf::Color _fg = { 170, 170, 170 };
	const sf::Color _textColor = { 0, 0, 0 };

	const sf::Color _bgHover = sf::Color::White;
	const sf::Color _fgHover = { 120, 120, 120 };
	const sf::Color _textColorHover = sf::Color::White;

	const sf::Color _bgClicked = { 200, 200, 200 };
	const sf::Color _fgClicked = { 100, 100, 100 };
	const sf::Color _textColorClicked = { 200, 200, 200 };

	const unsigned int _charSize = 56u;
	const float _padding = 4.f;
	const float _marginY = -20.f;

	// Data
	bool _clicked = false;
	sf::Vector2f _position;
	sf::Vector2f _size;
	State _state, _old;
	sf::Text _text;
	sf::VertexArray _shape;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(_shape, states);
		target.draw(_text, states);
	}

	bool pointInShape(const sf::Vector2f& point)
	{
		return
			point.x > _position.x - _size.x / 2 &&
			point.x < _position.x + _size.x / 2 &&
			point.y > _position.y - _size.y / 2 &&
			point.y < _position.y + _size.y / 2;
	}

public:
	Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& str, ResManager* res)
	{
		_position = position;
		_size = size;

		_state = State::Default;
		_old = State::Default;

		_shape.setPrimitiveType(sf::Quads);
		_shape.append(sf::Vertex({ position.x - size.x / 2, position.y - size.y / 2 }, _bg));
		_shape.append(sf::Vertex({ position.x + size.x / 2, position.y - size.y / 2 }, _bg));
		_shape.append(sf::Vertex({ position.x + size.x / 2, position.y + size.y / 2 }, _bg));
		_shape.append(sf::Vertex({ position.x - size.x / 2, position.y + size.y / 2 }, _bg));
		_shape.append(sf::Vertex({ position.x - size.x / 2 + _padding, position.y - size.y / 2 + _padding }, _fg));
		_shape.append(sf::Vertex({ position.x + size.x / 2 - _padding, position.y - size.y / 2 + _padding }, _fg));
		_shape.append(sf::Vertex({ position.x + size.x / 2 - _padding, position.y + size.y / 2 - _padding }, _fg));
		_shape.append(sf::Vertex({ position.x - size.x / 2 + _padding, position.y + size.y / 2 - _padding }, _fg));

		_text.setFont(res->fonts.font);
		_text.setCharacterSize(_charSize);
		_text.setFillColor(_textColor);
		changeString(str);
	}

	void changeString(const std::string& str)
	{
		_text.setString(str);
		auto tsize = _text.getGlobalBounds();
		_text.setPosition({ _position.x - tsize.width / 2, _position.y - tsize.height / 2 + _marginY });
	}

	bool clicked() const
	{
		return _clicked;
	}

	void reset()
	{
		_clicked = false;
		_state = State::Default;
	}

	void updateEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
		{
			if (pointInShape({ (float)event.mouseButton.x, (float)event.mouseButton.y }))
				_state = State::Clicked;
		}
		if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
		{
			if (pointInShape({ (float)event.mouseButton.x, (float)event.mouseButton.y }))
				_clicked = true;
			else
				_state = State::Default;
		}
	}

	void update(const sf::Vector2f& mousePos)
	{
		if (_state != State::Clicked)
		{
			if (pointInShape(mousePos))	_state = State::Hover;
			else						_state = State::Default;
		}
		
		if (_old != _state)
		{
			_old = _state;
			sf::Color bg, fg, textCol;
			switch (_state)
			{
			case Button::Default:
				bg = _bg;
				fg = _fg;
				textCol = _textColor;
				break;
			case Button::Hover:
				bg = _bgHover;
				fg = _fgHover;
				textCol = _textColorHover;
				break;
			case Button::Clicked:
				bg = _bgClicked;
				fg = _fgClicked;
				textCol = _textColorClicked;
				break;
			}
			for (int i = 0; i < 4; i++) _shape[i].color = bg;
			for (int i = 4; i < 8; i++) _shape[i].color = fg;
			_text.setFillColor(textCol);
		}
	}
};