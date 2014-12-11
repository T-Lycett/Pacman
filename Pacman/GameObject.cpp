#include "GameObject.h"


GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

void GameObject::Load()
{

}

void GameObject::Update(int elapsedTime)
{

}

void GameObject::Draw()
{
	if (texture != nullptr && sourceRect != nullptr && posRect != nullptr)
	{
		SpriteBatch::Draw(texture, sourceRect, posRect);
	}
}

void GameObject::SetPosition(Vector2& pos)
{
	if (posRect != nullptr)
	{
		posRect->X = pos.X;
		posRect->Y = pos.Y;
	}
}

Vector2 GameObject::GetPosition()
{
	Vector2 pos;
	pos.X = posRect->X;
	pos.Y = posRect->Y;
	return pos;
}