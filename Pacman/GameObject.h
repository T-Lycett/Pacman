#pragma once

#include "S2D\S2D.h"

using namespace S2D;

class GameObject
{
private:
	Texture2D* texture;
	Rect* sourceRect;
	Rect* posRect;
public:
	GameObject();
	virtual ~GameObject();

	virtual void Load();
	virtual void Update(int elapsedTime);
	virtual void Draw();

	virtual void SetPosition(Vector2& pos);
	virtual Vector2 GetPosition();
};

