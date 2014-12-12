#include "Player.h"

#include <sstream>

Player::Player() : _cPlayerSpeed(0.1f), _cPlayerFrameTime(250)
{
	currentFrameTime = 0;
	frame = 0;
	speedMultiplier = 1.0f;
	dead = false;
}

Player::~Player()
{
	delete boundingCircle;
	delete _stringPosition;
}

void Player::Load()
{
	texture = new Texture2D();
	texture->Load("Textures/Pacman.tga", false);
	posRect = new Rect(350.0f, 350.0f, 32, 32);
	sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	boundingCircle = new Circle(posRect->Center(), 16.0f);
	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);
}

void Player::Update(int elapsedTime)
{
	Input(elapsedTime);

	currentFrameTime += elapsedTime;

	if (currentFrameTime > _cPlayerFrameTime)
	{
		frame++;

		if (frame >= 2)
			frame = 0;

		currentFrameTime = 0;
	}

	sourceRect->X = sourceRect->Width * frame;

	sourceRect->Y = sourceRect->Height * direction;

	boundingCircle->Center(posRect->Center());

	CheckCollisions();
}

void Player::Draw()
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << posRect->X << " Y: " << posRect->Y << endl;
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);

	if (!dead)
	{
		SpriteBatch::Draw(texture, posRect, sourceRect); // Draws Pacman
	}
}

void Player::Input(int elapsedTime)
{
	Input::KeyboardState* state = Input::Keyboard::GetState();
	float playerSpeed = _cPlayerSpeed * elapsedTime * speedMultiplier;

	// Checks if D key is pressed
	if (state->IsKeyDown(Input::Keys::RIGHT))
	{
		posRect->X += playerSpeed; //Moves player across X axis
		direction = 0;
	}
	else if (state->IsKeyDown(Input::Keys::LEFT))
	{
		posRect->X -= playerSpeed; //Moves player across X axis
		direction = 2;
	}
	else if (state->IsKeyDown(Input::Keys::DOWN))
	{
		posRect->Y += playerSpeed; //Moves player across Y axis
		direction = 1;
	}
	else if (state->IsKeyDown(Input::Keys::UP))
	{
		posRect->Y -= playerSpeed; //Moves player across Y axis
		direction = 3;
	}

	if (state->IsKeyDown(Input::Keys::LEFTSHIFT))
	{
		speedMultiplier = 2.0f;
	}
	else {
		speedMultiplier = 1.0f;
	}
}

void Player::CheckCollisions()
{
	//wrap around viewport
	if (posRect->X > Graphics::GetViewportWidth())
		posRect->X = -sourceRect->Width;

	if (posRect->X < -sourceRect->Width)
		posRect->X = Graphics::GetViewportWidth();

	if (posRect->Y > Graphics::GetViewportHeight())
		posRect->Y = -sourceRect->Height;

	if (posRect->Y < -sourceRect->Height)
		posRect->Y = Graphics::GetViewportHeight();
}

const Circle& Player::GetBoundingCircle()
{
	return *boundingCircle;
}

bool Player::IsDead()
{
	return dead;
}

void Player::Kill()
{
	dead = true;
}