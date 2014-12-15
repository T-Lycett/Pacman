#include "Munchie.h"


Munchie::Munchie() : _cMunchieFrameTime(500)
{
	
	_frameCount = rand() % 1;
	_currentFrameTime = (rand() % 500) + 50;
	_eaten = false;
	
}


Munchie::~Munchie()
{
}


void Munchie::Load(Texture2D* texture)
{
	_texture = texture;
	_sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_posRect = new Rect(rand() % (Graphics::GetViewportWidth() - _sourceRect->Width), rand() % (Graphics::GetViewportHeight() - _sourceRect->Height), 32, 32);
}


void Munchie::Update(int elapsedTime)
{
	_currentFrameTime += elapsedTime;
	if (_currentFrameTime > _cMunchieFrameTime)
	{
		_frameCount++;

		if (_frameCount >= 2)
			_frameCount = 0;

		_currentFrameTime = 0;
	}

	_sourceRect->X = _sourceRect->Width * _frameCount;
}


void Munchie::OnCollected()
{
	_eaten = true;
}


bool Munchie::IsEaten()
{
	return _eaten;
}

const Rect& Munchie::GetBoundingRect()
{
	return *_posRect;
}