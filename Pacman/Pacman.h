#pragma once

#define GHOSTCOUNT 5


// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"
#include "Circle.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

struct Player 
{
	Rect* posRect;
	Rect* sourceRect;
	Texture2D* texture;
	int direction;
	int frame;
	int currentFrameTime;
	float speedMultiplier;
	Circle* boundingCircle;
	bool dead;
};

struct Enemy
{
	int frameCount;
	Rect* rect;
	Texture2D* texture;
	int currentFrameTime;
	bool eaten;
	Rect* posRect;
};

struct MovingEnemy
{
	Rect* posRect;
	Rect* sourceRect;
	Texture2D* texture;
	int direction;
	int directionTime;
	int currentDirectionTime;
	float speed;
	enum EnemyBehaviour behaviour;
	enum EnemyBehaviour previousBehaviour;
};

struct Menu
{
	Texture2D* background;
	Rect* rectangle;
	Vector2* stringPosition;
};

enum EnemyBehaviour
{
	MOVE_RANDOM,
	CHASE,
	PATROL,
};

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:
	
	Player* _pacman;
	Enemy* _munchies;
	MovingEnemy* _ghosts[GHOSTCOUNT];

	int _munchieCount;
	int _munchiesEaten;
	const float _cMunchieMutiplier;
	Texture2D* _munchieTexture;

	Menu* _startMenu;
	Menu* _pauseMenu;
	Menu* _gameOverMenu;

	// Position for String
	Vector2* _stringPosition;

	const float _cPacmanSpeed;
	const int _cPacmanFrameTime;
	const int _cMunchieFrameTime;

	const int _cEnemyMinDirectionTime;
	const float _cEnemyDistanceStartChase;
	const float _cEnemyDistanceStopChase;

	const int _cMunchieMoveTrigger;
	const float _cMunchieSpeed;
	bool _rKeyDown;

	bool _paused;
	bool _pKeyDown;
	bool _start;
	bool _spaceKeyDown;
	const Input::Keys _cPauseKey;

	SoundEffect* _pop;

	void Input(int elapsedTime, Input::KeyboardState & state, const Input::MouseState & mouseState);

	void CheckPaused(Input::KeyboardState & state, Input::Keys pauseKey);
	void CheckStart(Input::KeyboardState & state);

	void CheckViewportCollision();

	void UpdatePacman(int elapsedTime);

	void UpdateMunchies(int elapsedTime);

	void UpdateMunchie(Enemy & munchie, int elapsedTime);
	void CheckMunchieCollisions();

	void LoadMunchie(Enemy& munchie);

	void DeleteMunchie(Enemy* munchie);

	void UpdateGhosts(int elapsedTime);
	void CheckGhostCollision();

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[], int munchieCount);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};