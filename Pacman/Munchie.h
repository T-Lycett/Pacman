#pragma once
#include "GameObject.h"
#include "Player.h"
#include "Map.h"

class Munchie : public GameObject
{
private:
	bool _eaten;
	int _frameCount;
	int _currentFrameTime;
	const int _cMunchieFrameTime;

	SoundEffect* _pop;

	Map& _map;
public:
	Munchie(Map& map);
	~Munchie();

	void Load(Texture2D* texture);
	void Update(int elapsedTime);

	void OnCollected();
	bool IsEaten();
	const Rect& GetBoundingRect();
};