#include "MovingEnemy.h"


MovingEnemy::MovingEnemy(Map& map, EnemyBehaviour behaviour) : _cEnemyMinDirectionTime(100), _cSightDistance(200.0f), _cFrameTime(250), _map(map)
{
	_direction = 0;
	_directionTime = (rand() % 1500) + 1000;
	_currentDirectionTime = 0;
	_speed = 0.11f;
	_behaviour = behaviour;
	_frame = 0;
	_currentFrameTime = 0;
}


MovingEnemy::~MovingEnemy()
{
	delete _posRect;
	delete _sourceRect;
}


void MovingEnemy::Load(Texture2D* texture, Player* pacman)
{
	_texture = texture;
	_pacman = pacman;
	do
	{
		_posRect = new Rect(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight(), 20, 20);
	} while (Vector2::Distance(_posRect->Center(), _pacman->GetPosition().Center()) < _cSightDistance);
	_sourceRect = new Rect(0.0f, 0.0f, 20, 20);
	_lastKnownPlayerPos = _pacman->GetPosition().Center();
}


void MovingEnemy::Update(int elapsedTime)
{
	int currentDirection = _direction;

	if (_direction == 0)
	{
		//move right
		_posRect->X += _speed * elapsedTime;
	}
	else if (_direction == 2)
	{
		//move left
		_posRect->X -= _speed * elapsedTime;
	}
	else if (_direction == 1)
	{
		//move down
		_posRect->Y += _speed * elapsedTime;
	}
	else if (_direction == 3)
	{
		//move up
		_posRect->Y -= _speed * elapsedTime;
	}

	bool playerInSight = Vector2::Distance(_pacman->GetPosition().Center(), this->GetPosition().Center()) < _cSightDistance && _map.InLineOfSight(_pacman->GetPosition().Center(), this->GetPosition().Center());
	
	if (playerInSight)
		_lastKnownPlayerPos = _pacman->GetPosition().Center();

	if (_behaviour == EnemyBehaviour::MOVE_RANDOM)
	{
		if (_currentDirectionTime >= _directionTime)
		{
			_direction = rand() % 4;
		}

		if (_posRect->X + _posRect->Width >= Graphics::GetViewportWidth())
		{
			_direction = 2;
		}
		else if (_posRect->X <= 0)
		{
			_direction = 0;
		}
		else if (_posRect->Y + _posRect->Height >= Graphics::GetViewportHeight())
		{
			_direction = 3;
		}
		else if (_posRect->Y <= 0)
		{
			_direction = 1;
		}
		
		if (playerInSight && !_pacman->IsDead())
		{
			_previousBehaviour = _behaviour;
			_behaviour = EnemyBehaviour::CHASE;
		}
	}
	else if (_behaviour == EnemyBehaviour::CHASE)
	{
		if (!_pacman->IsDead())
		{

			MoveTowards(_lastKnownPlayerPos);
			
			if (_posRect->Contains(_lastKnownPlayerPos))
			{
				_behaviour = _previousBehaviour;
				_previousBehaviour = EnemyBehaviour::CHASE;
			}

		}
		else
		{
			_behaviour = _previousBehaviour;
			_previousBehaviour = EnemyBehaviour::CHASE;
		}
	}
	else if (_behaviour == EnemyBehaviour::PATROL)
	{
		if (_direction == 0)
		{
			if (_posRect->X + _posRect->Width >= Graphics::GetViewportWidth())
			{
				_direction = 3;
			}
		}
		if (_direction == 2)
		{
			if (_posRect->X <= 0)
			{
				_direction = 1;
			}
		}
		if (_direction == 1)
		{
			if (_posRect->Y + _posRect->Height >= Graphics::GetViewportHeight())
			{
				_direction = 0;
			}
		}
		if (_direction == 3)
		{
			if (_posRect->Y <= 0)
			{
				_direction = 2;
			}
		}
		if (playerInSight && !_pacman->IsDead())
		{
			_previousBehaviour = _behaviour;
			_behaviour = EnemyBehaviour::CHASE;
		}
	}

	if (currentDirection != _direction)
	{
		_currentDirectionTime = 0;
		_directionTime = (rand() % 1500) + 1000;
	}
	else
	{
		_currentDirectionTime += elapsedTime;
	}

	_sourceRect->Y = _sourceRect->Height * _direction;


	_currentFrameTime += elapsedTime;

	if (_currentFrameTime > _cFrameTime)
	{
		_frame++;

		if (_frame >= 2)
			_frame = 0;

		_currentFrameTime = 0;
	}

	_sourceRect->X = _sourceRect->Width * _behaviour * 2 + (_sourceRect->Width * _frame);
}


void MovingEnemy::MoveTowards(const Vector2& position)
{
	if (_currentDirectionTime > _cEnemyMinDirectionTime)
	{
		if (abs(_posRect->Center().X - position.X) > abs(_posRect->Center().Y - position.Y))
		{
			if (_posRect->Center().X > position.X)
			{
				_direction = 2;
			}
			else
			{
				_direction = 0;
			}
		}
		else
		{
			if (_posRect->Center().Y > position.Y)
			{
				_direction = 3;
			}
			else
			{
				_direction = 1;
			}
		}
	}
}


const Rect& MovingEnemy::GetBoundingRect()
{
	return *_posRect;
}