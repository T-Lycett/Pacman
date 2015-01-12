#include "GameObject.h"


GameObject::GameObject()
{
	_texture = nullptr;
	_sourceRect = nullptr;
	_posRect = nullptr;

	_draw = true;
}


GameObject::~GameObject()
{
}


void GameObject::Load(Texture2D* texture)
{

}


void GameObject::Update(int elapsedTime)
{

}


void GameObject::Draw()
{
	if (_draw)
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


void GameObject::SetDraw(bool draw)
{
	_draw = draw;
}