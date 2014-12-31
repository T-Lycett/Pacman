#include "Pacman.h"
#include "Circle.h"
#include <time.h>

#include <sstream>

Pacman::Pacman(int argc, char* argv[], int munchieCount) : Game(argc, argv), _cPauseKey(Input::Keys::P), _cMunchieMutiplier(1.5f)
{
	srand(time(nullptr));

	_pacman = new Player();

	_startMenu = new Menu();
	_pauseMenu = new Menu();
	_gameOverMenu = new Menu();

	_munchiesEaten = 0;
	_munchieCount = munchieCount;
	_munchies = new Munchie[_munchieCount];
	
	_paused = false;
	_pKeyDown = false;
	_start = false;
	_spaceKeyDown = false;
	_rKeyDown = false;

	for (int iii = 0; iii < GHOSTCOUNT; iii++)
	{
		_ghosts[iii] = new MovingEnemy();
	}

	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete[] _munchies;

	delete _munchieTexture;
	delete _pacmanTexture;
	delete _ghostTex;
	
	delete _pacman;

	delete _stringPosition;

	for (int iii = 0; iii < GHOSTCOUNT; iii++)
	{
		delete _ghosts[iii];
	}
	delete[] _ghosts;

	delete _startMenu->background;
	delete _startMenu->rectangle;
	delete _startMenu->stringPosition;
	delete _startMenu;

	delete _pauseMenu->background;
	delete _pauseMenu->rectangle;
	delete _pauseMenu->stringPosition;
	delete _pauseMenu;

	delete _gameOverMenu->background;
	delete _gameOverMenu->rectangle;
	delete _gameOverMenu->stringPosition;
	delete _gameOverMenu;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacmanTexture = new Texture2D();
	_pacmanTexture->Load("Textures/Pacman.tga", false);
	_pacman->Load(_pacmanTexture);

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	// Load Munchies
	_munchieTexture = new Texture2D();
	_munchieTexture->Load("Textures/Cherry.png", false);
	for (int iii = 0; iii < _munchieCount; iii++)
	{
		_munchies[iii].Load(_munchieTexture);
	}

	//load ghost
	_ghostTex = new Texture2D();
	_ghostTex->Load("Textures/ghost.png", false);
	for (int iii = 0; iii < GHOSTCOUNT; iii++)
	{
		_ghosts[iii]->Load(_ghostTex, _pacman);
	}

	// Set Menu Paramters
	Texture2D* menuTex = new Texture2D();
	menuTex->Load("Textures/Transparency.png", false);

	_startMenu->background = menuTex;
	_startMenu->rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(),
		Graphics::GetViewportHeight());
	_startMenu->stringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f,
		Graphics::GetViewportHeight() / 2.0f);

	_pauseMenu->background = menuTex;
	_pauseMenu->rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(),
		Graphics::GetViewportHeight());
	_pauseMenu->stringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f,
		Graphics::GetViewportHeight() / 2.0f);

	_gameOverMenu->background = menuTex;
	_gameOverMenu->rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(),
		Graphics::GetViewportHeight());
	_gameOverMenu->stringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f,
		Graphics::GetViewportHeight() / 2.0f);

	
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	Input::MouseState* mouseState = Input::Mouse::GetState();

	if (!_pacman->IsDead())
	{
		CheckStart(*keyboardState);

		CheckPaused(*keyboardState, _cPauseKey);
	}
	
	if (!_paused && _start)
	{
		Input(elapsedTime, *keyboardState, *mouseState);

		_pacman->Update(elapsedTime);

		UpdateMunchies(elapsedTime);

		CheckMunchieCollisions();

		UpdateGhosts(elapsedTime);

		CheckGhostCollision();
		
	}
}

void Pacman::Draw(int elapsedTime)
{
	SpriteBatch::BeginDraw(); // Starts Drawing


	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacman->GetPosition().X << " Y: " << _pacman->GetPosition().X << endl;
	if (elapsedTime)
		stream << "FPS: " << 1000 / elapsedTime;
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);

	for (int iii = 0; iii < _munchieCount; iii++)
	{
		if (!_munchies[iii].IsEaten())
			_munchies[iii].Draw();
	}

	for (int iii = 0; iii < GHOSTCOUNT; iii++)
	{
		_ghosts[iii]->Draw();
	}

	if (_paused)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED!";
		SpriteBatch::Draw(_pauseMenu->background, _pauseMenu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _pauseMenu->stringPosition,
			Color::Red);
	}

	if (!_start)
	{
		std::stringstream menuStream;
		menuStream << "PRESS SPACE!";
		SpriteBatch::Draw(_startMenu->background, _startMenu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _startMenu->stringPosition,
			Color::Red);
	}
	
	

	if (_pacman->IsDead())
	{
		std::stringstream menuStream;
		menuStream << "GAME OVER!";
		SpriteBatch::Draw(_gameOverMenu->background, _gameOverMenu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _gameOverMenu->stringPosition,
			Color::Red);
	}
	
	_pacman->Draw();

	SpriteBatch::EndDraw(); // Ends Drawing
}

void Pacman::Input(int elapsedTime, Input::KeyboardState & state, const Input::MouseState & mouseState)
{
	if (state.IsKeyDown(Input::Keys::R) && !_rKeyDown)
	{
		for (int iii = 0; iii < _munchieCount; iii++)
		{
			float x = rand() % (Graphics::GetViewportWidth() - _munchies[iii].GetBoundingRect().Width);
			float y = rand() % (Graphics::GetViewportHeight() - _munchies[iii].GetBoundingRect().Height);
			_munchies[iii].SetPosition(x, y);
		}


		_rKeyDown = true;
	}
	if (state.IsKeyUp(Input::Keys::R))
	{
		_rKeyDown = false;
	}
}

void Pacman::CheckPaused(Input::KeyboardState & state, Input::Keys pauseKey) 
{
	if (state.IsKeyDown(pauseKey) && !_pKeyDown)
	{
		_pKeyDown = true;
		_paused = !_paused;
	}
	if (state.IsKeyUp(pauseKey))
	{
		_pKeyDown = false;
	}
}

void Pacman::CheckStart(Input::KeyboardState & state)
{
	if (state.IsKeyDown(Input::Keys::SPACE) && !_spaceKeyDown && _start == false)
	{
		_spaceKeyDown = true;
		_start = true;
	}
}

void Pacman::UpdateMunchies(int elapsedTime)
{
	if (_munchieCount == _munchiesEaten)
	{
		delete[] _munchies;

		_munchieCount = _munchieCount * _cMunchieMutiplier;
		_munchiesEaten = 0;

		_munchies = new Munchie[_munchieCount];

		for (int iii = 0; iii < _munchieCount; iii++)
		{
			_munchies[iii].Load(_munchieTexture);
		}
	}
	for (int iii = 0; iii < _munchieCount; iii++)
	{
		_munchies[iii].Update(elapsedTime);
	}
}

void Pacman::CheckMunchieCollisions()
{
	for (int iii = 0; iii < _munchieCount; iii++)
	{
		if (!_munchies[iii].IsEaten())
		{
			if (_pacman->GetBoundingCircle().Intersects(_munchies[iii].GetBoundingRect()))
			{
				_munchies[iii].OnCollected();
				_munchiesEaten++;
			}
		}
	}
}

void Pacman::UpdateGhosts(int elapsedTime)
{
	for (int iii = 0; iii < GHOSTCOUNT; iii++)
	{
		_ghosts[iii]->Update(elapsedTime);
	}
}

void Pacman::CheckGhostCollision()
{
	for (int iii = 0; iii < GHOSTCOUNT; iii++)
	{
		if (_pacman->GetBoundingCircle().Intersects(_ghosts[iii]->GetBoundingRect()))
			_pacman->Kill();
	}
}