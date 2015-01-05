#pragma once
#include "GameObject.h"
#include "Player.h"

enum EnemyBehaviour
{
	MOVE_RANDOM,
	CHASE,
	PATROL,
};

class MovingEnemy : public GameObject
{
private:
	int _direction;
	int _directionTime;
	int _currentDirectionTime;
	float _speed;
	enum EnemyBehaviour _behaviour;
	enum EnemyBehaviour _previousBehaviour;

	const int _cEnemyMinDirectionTime;
	const float _cEnemyDistanceStartChase;
	const float _cEnemyDistanceStopChase;

	Player* _pacman;
	Map& _map;

	Vector2* _lastKnownPlayerPos;

	void MoveTowards(const Vector2& position);
public:
	MovingEnemy(Map& map);
	~MovingEnemy();

	void Load(Texture2D* texture, Player* pacman);
	void Update(int elapsedTime);

	const Rect& GetBoundingRect();
};

