#include "Player.h"
#include "Tile.h"

#include <sstream>

Player::Player(Map& map) : _cPlayerSpeed(0.1f), _cPlayerFrameTime(250), _map(map)
{
	_currentFrameTime = 0;
	_frame = 0;
	_speedMultiplier = 1.0f;
	_dead = false;
	_boundingCircle = nullptr;
	_direction = -1;
	_lastPosRect = nullptr;
}

Player::~Player()
{
	delete _boundingCircle;
	delete _lastPosRect;
}

void Player::Load(Texture2D* texture)
{
	_texture = texture;
	_posRect = new Rect(350.0f, 350.0f, 32, 32);
	_sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_boundingCircle = new Circle(_posRect->Center(), 16.0f);
	_lastPosRect = new Rect();
	*_lastPosRect = *_posRect;
}

void Player::Update(int elapsedTime)
{
	if (_dead)
	{
		_draw = false;
		return;
	}

	_lastPosRect->X = _posRect->X;
	_lastPosRect->Y = _posRect->Y;

	Move(elapsedTime);

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

	if (_direction >= 0)
		_sourceRect->Y = _sourceRect->Height * _direction;

	_boundingCircle->Center(_posRect->Center());

	CheckCollisions();
}

void Player::Input(int elapsedTime)
{
	Input::KeyboardState* state = Input::Keyboard::GetState();

	if (state->IsKeyDown(Input::Keys::RIGHT))
	{
		_direction = 0;
	}
	else if (state->IsKeyDown(Input::Keys::LEFT))
	{
		_direction = 2;
	}
	else if (state->IsKeyDown(Input::Keys::DOWN))
	{
		_direction = 1;
	}
	else if (state->IsKeyDown(Input::Keys::UP))
	{
		_direction = 3;
	}
	else
	{
		_direction = -1;
	}

	if (state->IsKeyDown(Input::Keys::LEFTSHIFT))
	{
		_speedMultiplier = 2.0f;
	}
	else {
		_speedMultiplier = 1.0f;
	}
}


void Player::Move(int elapsedTime)
{
	float playerSpeed = _cPlayerSpeed * elapsedTime * _speedMultiplier;

	switch (_direction)
	{
	case 0:
		_posRect->X += playerSpeed; //Moves player across X axis
		break;
	case 2:
		_posRect->X -= playerSpeed; //Moves player across X axis
		break;
	case 1:
		_posRect->Y += playerSpeed; //Moves player across Y axis
		break;
	case 3:
		_posRect->Y -= playerSpeed; //Moves player across Y axis
		break;
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

	int x = _posRect->X / Tile::SIZE;
	int y = _posRect->Y / Tile::SIZE;
	for (int iii = x; iii <= (int) (x + (_posRect->Width / Tile::SIZE)); iii++)
	{
		for (int jjj = y; jjj <= (int) (y + (_posRect->Height / Tile::SIZE)); jjj++)
		{
			if (_map.IsValidTile(iii, jjj))
			{
				Tile& tile = _map.GetTile(iii, jjj);
				if (tile.IsCollidable() && _boundingCircle->Intersects(tile.GetBoundingRect()))
				{
					_posRect->X = _lastPosRect->X;
					_posRect->Y = _lastPosRect->Y;
				}
			}
		}
	}
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