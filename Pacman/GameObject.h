#pragma once

#include "S2D\S2D.h"

using namespace S2D;

class GameObject
{
protected:
	Texture2D* texture;
	Rect* sourceRect;
	Rect* posRect;
public:
	GameObject();
	virtual ~GameObject();

	virtual void Load();
	virtual void Update(int elapsedTime);
	virtual void Draw();

	virtual void SetPosition(float x, float y);
	virtual const Rect& GetPosition();
};

