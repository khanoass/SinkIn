#pragma once

#include "Player.h"
#include "Map.h"
#include "Level.h"
#include "Sequence.hpp"

class Levels : public sf::Drawable
{
private:
	// Data
	sf::Vector2f _screenCenter;
	std::vector<std::shared_ptr<Level>> _levels;
	std::vector<Sequence> _sequences;
	int _i;
	bool _sequence = false;
	bool _finished = false;
	bool _nextLevel = false;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
	Levels(const std::vector<std::string>& mapfilenames, const std::vector<std::string>& sequencesfilenames, const sf::Vector2f& center, ResManager* res);

	bool tutorial();
	bool tutorialStageChanged();
	int tutorialStage();

	bool sequence();
	bool finished();
	bool gameOver();
	void restartLevel();
	void nextLevel();
	bool isNextLevel();

	std::shared_ptr<Player> player();
	std::shared_ptr<Map> map();

	void setVignettePosition(const sf::Vector2f& pos);

	void updateEvent(const sf::Event& event, float dt, const sf::Vector2f& mousePos);
	virtual void update(float dt, const sf::Vector2f& mousePos);
};