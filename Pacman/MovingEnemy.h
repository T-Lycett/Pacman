#pragma once
#include "GameObject.h"
#include "Player.h"

enum EnemyBehaviour
{
	MOVE_RANDOM,
	PATROL,
	CHASE,
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
	const float _cSightDistance;

	int _frame;
	int _currentFrameTime;
	const int _cFrameTime;

	Player* _pacman;
	Map& _map;

	Vector2 _lastKnownPlayerPos;

	void MoveTowards(const Vector2& position);
public:
	MovingEnemy(Map& map);
	~MovingEnemy();

	void Load(Texture2D* texture, Player* pacman);
	void Update(int elapsedTime);

	const Rect& GetBoundingRect();
};

