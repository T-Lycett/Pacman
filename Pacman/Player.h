#pragma once

#include "GameObject.h"
#include "Circle.h"
#include "S2D\S2D.h"

using namespace S2D;

class Player : public GameObject
{
private:
	int direction;
	int frame;
	int currentFrameTime;
	float speedMultiplier;
	Circle* boundingCircle;
	bool dead;

	// Position for String
	Vector2* _stringPosition;

	const float _cPlayerSpeed;
	const int _cPlayerFrameTime;

	void Input(int elapsedTime);
	void CheckCollisions();
public:
	Player();
	~Player();

	void Load();
	void Update(int elapsedTime);
	void Draw();

	const Circle& GetBoundingCircle();
	bool IsDead();
	void Kill();
	
};