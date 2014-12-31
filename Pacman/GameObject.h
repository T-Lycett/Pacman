#pragma once

#include "S2D\S2D.h"

using namespace S2D;

class GameObject
{
protected:
	Texture2D* _texture;
	Rect* _sourceRect;
	Rect* _posRect;

	bool _draw;
public:
	GameObject();
	virtual ~GameObject();

	virtual void Load(Texture2D* texture);
	virtual void Update(int elapsedTime);
	virtual void Draw();

	virtual void SetPosition(float x, float y);
	virtual const Rect& GetPosition() const;

	void SetDraw(bool draw);
};

