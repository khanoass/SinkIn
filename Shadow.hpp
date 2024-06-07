#pragma once

#include "Enemy.hpp"
#include "Random.hpp"

class Shadow : public Enemy
{
private:
	enum State
	{
		Strolling,
		Aggro,
		Preparing,
		Dashing,
		Hurt
	};

	const float _rotInc = 150.f;
	const float _angleThresh = 1.f;
	const float _bugTimeout = 15.f;
	const float _aggroTimeout = 60.f;
	const float _hurtTime = 0.5f;
	const float _prepareTime = 0.7f;
	const float _dashForce = 1500.f;
	const float _dashRange = 200.f;
	const float _pointMargin = 100.f;

	bool _pointChosen;
	float _maxDist;
	State _state;
	sf::Clock _timoutClock, _aggroClock, _hurtClock, _prepareClock;

	// Textures
	sf::Texture* _bodySheet, *_eyesStrolling, *_eyesAggro, *_eyesPreparing, *_eyesDashing, *_eyesHurt;
	Animation _bodyAnimation;
	sf::Sprite _body, _eyes;

	void rotateTowards(const sf::Vector2f& point, float dt)
	{
		float angle = vm::angleTwo(_direction, point - _position);
		float newa = angle;
		if (angle > 0) newa -= _rotInc * dt;
		else		   newa += _rotInc * dt;
		_direction = vm::rotateVector(_direction, angle-newa);
	}

	bool reachedRotationTarget(const sf::Vector2f& point)
	{
		float angle = vm::angleTwo(_direction, point - _position);
		return angle < _angleThresh && angle > -_angleThresh && vm::dot(_direction, point - _position) > 0;
	}

	// States
	void updateStrolling(const std::shared_ptr<Player>& player, float dt)
	{
		if (!_moving)
		{
			if (!_pointChosen)
			{
				// Choose random point toward player
				do
					_movTarget = { 
						Random::fRand(_border[0].x + _pointMargin, _border[0].y - _pointMargin), 
						Random::fRand(_border[1].x + _pointMargin, _border[1].y - _pointMargin) 
					};
				while (vm::dist(_movTarget, player->position()) > _maxDist);
				_pointChosen = true;
				_timoutClock.restart();
			}
			else if (reachedRotationTarget(_movTarget))
			{
				startMoving(_movTarget);
			}
			else
			{
				rotateTowards(_movTarget, dt);

				Logger::log({ "rotating" });
			}
		}
		else
		{
			Logger::log({ "moving" });

			// Avoid bugs with walls collisions
			if (_pointChosen && _timoutClock.getElapsedTime().asSeconds() > _bugTimeout)
			{
				_pointChosen = false;
				_moving = false;

				Logger::log({ "timed out" });
			}
		}

		if (reachedTarget())
		{
			// RNG can become aggro for no reason
			int r = Random::iRand(0, 10);
			if (r > 7)
			{
				changeState(Shadow::Aggro);
				return;
			}

			// Restart when there
			_pointChosen = false;
			_moving = false;

			Logger::log({ "restarting" });
		}
	}

	void updateAggro(const std::shared_ptr<Player>& player)
	{
		startMoving(player->position());

		if (vm::dist(_position, player->position()) < _dashRange)
		{
			// RNG can prepare when close to player
			int r = Random::iRand(0, 1000);
			if (r > 998)
			{
				changeState(Shadow::Preparing);
				_prepareClock.restart();
				_moving = false;
				return;
			}
		}

		if (_aggroClock.getElapsedTime().asSeconds() > _aggroTimeout)
			changeState(State::Strolling);
	}

	void updatePreparing()
	{
		if (_prepareClock.getElapsedTime().asSeconds() > _prepareTime)
		{
			changeState(Shadow::Dashing);
			applyImpulse(_dashForce);
		}
	}

	void updateDashing(const std::shared_ptr<Player>& player)
	{
		if (!_impulsing)
		{
			changeState(Shadow::Aggro);
			_aggroClock.restart();
		}
	}

	void updateHurt()
	{
		if (_hurtClock.getElapsedTime().asSeconds() > _hurtTime)
		{
			changeState(Shadow::Aggro);
			_aggroClock.restart();
		}
	}

	void changeState(State to)
	{
		_state = to;
		
		// Updating texture
		_tex.clear(sf::Color::Transparent);

		switch (_state)
		{
		case Shadow::Strolling: _eyes.setTexture(*_eyesStrolling);	break;
		case Shadow::Aggro:		_eyes.setTexture(*_eyesAggro);		break;
		case Shadow::Preparing:	_eyes.setTexture(*_eyesPreparing);	break;
		case Shadow::Dashing:	_eyes.setTexture(*_eyesDashing);	break;
		case Shadow::Hurt:		_eyes.setTexture(*_eyesHurt);		break;
		}

		_tex.draw(_body);
		_tex.draw(_eyes);
		_tex.display();
	}

public:
	Shadow(const sf::Vector2f& position, ResManager* res)
		: Enemy(position, { 60, 60 }, &res->textures.shadow_death, { 7, 1 }, { 100, 100 }, 100, res)
	{
		_pointChosen = false;
		_state = Strolling;
		_maxDist = 400.f;
		_direction = { 1.f, 0 };

		_bodySheet = &res->textures.shadow_body;
		_eyesStrolling = &res->textures.shadow_eyes_strolling;
		_eyesAggro = &res->textures.shadow_eyes_aggro;
		_eyesPreparing = &res->textures.shadow_eyes_preparing;
		_eyesDashing = &res->textures.shadow_eyes_dashing;
		_eyesHurt = &res->textures.shadow_eyes_hurt;

		_bodyAnimation.setSpritesheet(sf::IntRect(0, 0, 60, 60), {5, 1}, 0.05f);
		_bodyAnimation.start();

		_body.setTexture(*_bodySheet);
		_body.setTextureRect(_bodyAnimation.frame());
		_body.setPosition(0, 0);
		_eyes.setTexture(*_eyesStrolling);
		_body.setPosition(0, 0);

		_range = 50;
		_speed = 100;
		_friction = 6.f;
		_damage = 25.f;
	}

	virtual void updateEnemy(float dt, const std::shared_ptr<Player>& player) override
	{
		if (_bodyAnimation.changed())
		{
			_body.setTextureRect(_bodyAnimation.frame());
			_tex.clear(sf::Color::Transparent);
			_tex.draw(_body);
			_tex.draw(_eyes);
			_tex.display();
		}

		if (_justHit)
		{
			_justHit = false;
			_moving = false;
			changeState(State::Hurt);
			_hurtClock.restart();
			return;
		}
		if (_justTouched)
		{
			_justTouched = false;
			changeState(State::Aggro);
			_aggroClock.restart();
			return;
		}

		switch (_state)
		{
		case Shadow::Strolling:
			updateStrolling(player, dt);
			break;
		case Shadow::Aggro:
			updateAggro(player);
			break;
		case Shadow::Preparing:
			updatePreparing();
			break;
		case Shadow::Dashing:
			updateDashing(player);
			break;
		case Shadow::Hurt:
			updateHurt();
			break;
		}
	}
};