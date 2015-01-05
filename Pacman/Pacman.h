#pragma once

#define GHOSTCOUNT 1


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
#include "Player.h"
#include "Munchie.h"
#include "MovingEnemy.h"
#include "Map.h"
#include "Tile.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;


struct Menu
{
	Texture2D* background;
	Rect* rectangle;
	Vector2* stringPosition;
};

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:

	Map* _map;
	const int _cMapHeight, _cMapWidth;
	
	Player* _pacman;
	Munchie* _munchies;
	MovingEnemy* _ghosts[GHOSTCOUNT];

	// Position for String
	Vector2* _stringPosition;

	int _munchieCount;
	int _munchiesEaten;
	const float _cMunchieMutiplier;

	bool _fogOfWar;
	int _fogOfWarDistance;
	bool _fKeyDown;

	Texture2D* _munchieTexture;
	Texture2D* _pacmanTexture;
	Texture2D* _ghostTex;

	Menu* _startMenu;
	Menu* _pauseMenu;
	Menu* _gameOverMenu;

	bool _rKeyDown;

	bool _paused;
	bool _pKeyDown;
	bool _start;
	bool _spaceKeyDown;
	const Input::Keys _cPauseKey;

	void Input(int elapsedTime, Input::KeyboardState & state, const Input::MouseState & mouseState);

	void CheckPaused(Input::KeyboardState & state, Input::Keys pauseKey);
	void CheckStart(Input::KeyboardState & state);

	void UpdatePacman(int elapsedTime);

	void UpdateMunchies(int elapsedTime);

	void CheckMunchieCollisions();

	void UpdateGhosts(int elapsedTime);
	void CheckGhostCollision();

	void CheckFogOfWar(Input::KeyboardState& state);

	void UpdateFogOfWar();

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