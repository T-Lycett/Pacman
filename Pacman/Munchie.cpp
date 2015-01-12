#include "Munchie.h"


Munchie::Munchie(Map& map) : _cMunchieFrameTime(500), _map(map)
{
	
	_frameCount = rand() % 1;
	_currentFrameTime = (rand() % 500) + 50;
	_eaten = false;
	_draw = false;
}


Munchie::~Munchie()
{
	delete _sourceRect;
	delete _posRect;
	delete _pop;
}


void Munchie::Load(Texture2D* texture)
{
	_texture = texture;
	_sourceRect = new Rect(0.0f, 0.0f, 12, 12);
	do
	{
		if (_posRect != nullptr)
			delete _posRect;

		_posRect = new Rect(rand() % (Graphics::GetViewportWidth() - _sourceRect->Width), rand() % (Graphics::GetViewportHeight() - _sourceRect->Height), 12, 12);
	} while (_map.GetTileAtPosition(_posRect->X, _posRect->Y)->IsCollidable());

	//load sounds
	_pop = new SoundEffect();
	_pop->Load("Sounds/pop.wav");
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
	Audio::Play(_pop);
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