#include "GameObject.h"


GameObject::GameObject()
{
	_texture = nullptr;
	_sourceRect = nullptr;
	_posRect = nullptr;
}


GameObject::~GameObject()
{
	delete _sourceRect;
	delete _posRect;
}


void GameObject::Load(Texture2D* texture)
{

}


void GameObject::Update(int elapsedTime)
{

}


void GameObject::Draw()
{
	SpriteBatch::Draw(_texture, _posRect, _sourceRect);
}

void GameObject::SetPosition(float x, float y)
{
	if (_posRect != nullptr)
	{
		_posRect->X = x;
		_posRect->Y = y;
	}
}

const Rect& GameObject::GetPosition() const
{
	return *_posRect;
}