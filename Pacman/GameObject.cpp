#include "GameObject.h"


GameObject::GameObject()
{
	texture = nullptr;
	sourceRect = nullptr;
	posRect = nullptr;
}


GameObject::~GameObject()
{
	delete texture;
	delete sourceRect;
	delete posRect;
}

void GameObject::Load()
{

}

void GameObject::Update(int elapsedTime)
{

}

void GameObject::Draw()
{
	if (texture != nullptr && sourceRect != nullptr)
	{
		SpriteBatch::Draw(texture, sourceRect, posRect);
	}
}

void GameObject::SetPosition(float x, float y)
{
	if (posRect != nullptr)
	{
		posRect->X = x;
		posRect->Y = y;
	}
}

const Rect& GameObject::GetPosition()
{
	return *posRect;
}