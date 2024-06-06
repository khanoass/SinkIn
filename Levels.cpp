#include "Levels.h"

void Levels::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(!_sequences[_i].finished())
		target.draw(_sequences[_i], states);
	else
		target.draw(*_levels[_i], states);
}

Levels::Levels(const std::vector<std::string>& mapfilenames, const std::vector<std::string>& sequencesfilenames, const sf::Vector2f& center, ResManager* res)
{
	_screenCenter = center;

	for (const auto& f : mapfilenames)
		_levels.push_back(std::make_shared<Level>(f, center, res));

	for (const auto& f : sequencesfilenames)
		_sequences.push_back(Sequence(f, _screenCenter, res));

	_i = 0;
	_sequences[_i].start();
	_levels[_i]->start();
	_sequence = true;
}

void Levels::inc()
{
	_i++;
	_sequences[_i].start();
}

bool Levels::sequence()
{
	return _sequence;
}

std::shared_ptr<Player> Levels::player()
{
	return _levels[_i]->player();
}

std::shared_ptr<Map> Levels::map()
{
	return _levels[_i]->map();
}

void Levels::updateEvent(const sf::Event& event, float dt, const sf::Vector2f& mousePos)
{
	if (!_sequences[_i].finished())
	{
		_sequences[_i].updateEvent(event);
		if (_sequences[_i].finished())
			_sequence = false;
		return;
	}

	_levels[_i]->updateEvent(event, dt, mousePos);
}

void Levels::setVignettePosition(const sf::Vector2f& pos)
{
	map()->getTexShaderPtr()->setUniform("playerPos", sf::Glsl::Vec2(pos));
}

void Levels::update(float dt, const sf::Vector2f& mousePos)
{
	if (!_sequences[_i].finished())
	{
		_sequences[_i].update(dt, mousePos);
		return;
	}

	_levels[_i]->update(dt, mousePos);

	map()->getTexShaderPtr()->setUniform("screenSize", sf::Glsl::Vec2({ _screenCenter.x * 2, _screenCenter.y * 2 }));

	//map()->getTexShaderPtr()->setUniform("radius", 1.f);

	if (player()->boosted())	map()->getTexShaderPtr()->setUniform("radius", 0.75f);
	else					map()->getTexShaderPtr()->setUniform("radius", 0.5f);
}