#pragma once

#include "GameObject.h"
#include "Circle.h"
#include "S2D\S2D.h"
#include "Map.h"

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
	Rect* _lastPosRect;

	Map& _map;

	const float _cPlayerSpeed;
	const int _cPlayerFrameTime;

	void Input(int elapsedTime);
	void Move(int elapsedTime);
	void CheckCollisions();
public:
	Player(Map& map);
	~Player();

	void Load(Texture2D* texture);
	void Update(int elapsedTime);

	const Circle& GetBoundingCircle();
	bool IsDead();
	void Kill();
	
};