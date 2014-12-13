#pragma once

#include "GameObject.h"
#include "Circle.h"
#include "S2D\S2D.h"

using namespace S2D;

class Player : public GameObject
{
private:
	int _direction;
	int _frame;
	int _currentFrameTime;
	float _speedMultiplier;
	Circle* _boundingCircle;
	bool _dead;

	// Position for String
	Vector2* _stringPosition;

	const float _cPlayerSpeed;
	const int _cPlayerFrameTime;

	void Input(int elapsedTime);
	void CheckCollisions();
public:
	Player();
	~Player();

	void Load(Texture2D* texture);
	void Update(int elapsedTime);
	void Draw();

	const Circle& GetBoundingCircle();
	bool IsDead();
	void Kill();
	
};