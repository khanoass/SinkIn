#include "Levels.h"

void Levels::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (_i < _levels.size())
	{
		if (!_sequences[_i].finished())
			target.draw(_sequences[_i], states);
		else
			target.draw(*_levels[_i], states);
	}
}

Levels::Levels(const std::vector<std::string>& mapfilenames, const std::vector<std::string>& sequencesfilenames, const sf::Vector2f& center, ResManager* res)
{
	_screenCenter = center;

	for (int i = 0; i < mapfilenames.size(); i++)
		_levels.push_back(std::make_shared<Level>(mapfilenames[i], center, res, i == 0));

	for (const auto& f : sequencesfilenames)
		_sequences.push_back(Sequence(f, _screenCenter, res));

	_i = 0;
	_sequences[_i].start();
	_levels[_i]->start();
	_sequence = true;
}

bool Levels::sequence()
{
	return _sequence;
}

bool Levels::finished()
{
	return _finished;
}

bool Levels::gameOver()
{
	if (_i >= _levels.size()) return false;
	return _levels[_i]->isGameOver();
}

void Levels::restartLevel()
{
	if (_i < _levels.size())
		_levels[_i]->restart();
}

void Levels::nextLevel()
{
	_sequences[_i].start();
	_levels[_i]->start();
	_sequence = true;
}

bool Levels::isNextLevel()
{
	if (_nextLevel)
	{
		_nextLevel = false;
		return true;
	}
	return false;
}

bool Levels::tutorial()
{
	return _i == 0;
}

bool Levels::tutorialStageChanged()
{
	if (_i >= _levels.size()) return false;
	if (!tutorial()) return false;
	return _levels[_i]->tutorialStageChanged();
}

int Levels::tutorialStage()
{
	if (_i >= _levels.size()) return false;
	if (!tutorial()) return -1;
	return _levels[_i]->tutorialStage();
}

std::shared_ptr<Player> Levels::player()
{
	if (_i >= _levels.size()) return nullptr;
	return _levels[_i]->player();
}

std::shared_ptr<Map> Levels::map()
{
	if (_i >= _levels.size()) return nullptr;
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
	if(map() != nullptr)
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

	// Next level
	if (_levels[_i]->passed())
	{
		_i++;
		if (_i >= _levels.size())
		{
			_finished = true;
			return;
		}
		else
			_nextLevel = true;
	}

	map()->getTexShaderPtr()->setUniform("screenSize", sf::Glsl::Vec2({ _screenCenter.x * 2, _screenCenter.y * 2 }));

	//map()->getTexShaderPtr()->setUniform("radius", 1.f);

	if (player()->boosted())	map()->getTexShaderPtr()->setUniform("radius", 0.75f);
	else						map()->getTexShaderPtr()->setUniform("radius", 0.5f);
}