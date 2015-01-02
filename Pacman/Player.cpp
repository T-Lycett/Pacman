#include "Player.h"

#include <sstream>

Player::Player() : _cPlayerSpeed(0.1f), _cPlayerFrameTime(250)
{
	_currentFrameTime = 0;
	_frame = 0;
	_speedMultiplier = 1.0f;
	_dead = false;
	_boundingCircle = nullptr;
}

Player::~Player()
{
	delete _boundingCircle;
}

void Player::Load(Texture2D* texture)
{
	_texture = texture;
	_posRect = new Rect(350.0f, 350.0f, 32, 32);
	_sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_boundingCircle = new Circle(_posRect->Center(), 16.0f);
}

void Player::Update(int elapsedTime)
{
	if (_dead)
	{
		_draw = false;
		return;
	}

	Input(elapsedTime);

	_currentFrameTime += elapsedTime;

	if (_currentFrameTime > _cPlayerFrameTime)
	{
		_frame++;

		if (_frame >= 2)
			_frame = 0;

		_currentFrameTime = 0;
	}

	_sourceRect->X = _sourceRect->Width * _frame;

	_sourceRect->Y = _sourceRect->Height * _direction;

	_boundingCircle->Center(_posRect->Center());

	CheckCollisions();
}

void Player::Input(int elapsedTime)
{
	Input::KeyboardState* state = Input::Keyboard::GetState();
	float playerSpeed = _cPlayerSpeed * elapsedTime * _speedMultiplier;

	// Checks if D key is pressed
	if (state->IsKeyDown(Input::Keys::RIGHT))
	{
		_posRect->X += playerSpeed; //Moves player across X axis
		_direction = 0;
	}
	else if (state->IsKeyDown(Input::Keys::LEFT))
	{
		_posRect->X -= playerSpeed; //Moves player across X axis
		_direction = 2;
	}
	else if (state->IsKeyDown(Input::Keys::DOWN))
	{
		_posRect->Y += playerSpeed; //Moves player across Y axis
		_direction = 1;
	}
	else if (state->IsKeyDown(Input::Keys::UP))
	{
		_posRect->Y -= playerSpeed; //Moves player across Y axis
		_direction = 3;
	}

	if (state->IsKeyDown(Input::Keys::LEFTSHIFT))
	{
		_speedMultiplier = 2.0f;
	}
	else {
		_speedMultiplier = 1.0f;
	}
}

void Player::CheckCollisions()
{
	//wrap around viewport
	if (_posRect->X > Graphics::GetViewportWidth())
		_posRect->X = -_sourceRect->Width;

	if (_posRect->X < -_sourceRect->Width)
		_posRect->X = Graphics::GetViewportWidth();

	if (_posRect->Y > Graphics::GetViewportHeight())
		_posRect->Y = -_sourceRect->Height;

	if (_posRect->Y < -_sourceRect->Height)
		_posRect->Y = Graphics::GetViewportHeight();
}

const Circle& Player::GetBoundingCircle()
{
	return *_boundingCircle;
}

bool Player::IsDead()
{
	return _dead;
}

void Player::Kill()
{
	_dead = true;
}