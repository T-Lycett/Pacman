#include "Pacman.h"
#include "Circle.h"
#include <time.h>

#include <sstream>

Pacman::Pacman(int argc, char* argv[], int munchieCount) : Game(argc, argv), _cMunchieFrameTime(500), _cMunchieMoveTrigger(100), _cMunchieSpeed(0.1), _cPauseKey(Input::Keys::P), _cEnemyMinDirectionTime(100), _cEnemyDistanceStartChase(200.0f), _cEnemyDistanceStopChase(250.0f), _cMunchieMutiplier(1.5f)
{
	srand(time(nullptr));

	_pacman = new Player();

	_startMenu = new Menu();
	_pauseMenu = new Menu();
	_gameOverMenu = new Menu();

	_munchiesEaten = 0;
	_munchieCount = munchieCount;
	_munchies = new Enemy[munchieCount];
	
	_paused = false;
	_pKeyDown = false;
	_start = false;
	_spaceKeyDown = false;
	_rKeyDown = false;

	for (int iii = 0; iii < GHOSTCOUNT; iii++)
	{
		_ghosts[iii] = new MovingEnemy();
		_ghosts[iii]->direction = 0;
		_ghosts[iii]->directionTime = (rand() % 1500) + 1000;
		_ghosts[iii]->currentDirectionTime = 0;
		_ghosts[iii]->speed = 0.2f;
		_ghosts[iii]->behaviour = static_cast<EnemyBehaviour>((iii % 2) + 1);
		//_ghosts[iii]->behaviour = EnemyBehaviour::CHASE;
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

	for (int iii = 0; iii < _munchieCount; iii++)
	{
		DeleteMunchie(&_munchies[iii]);
	}
	delete[] _munchies;

	delete _munchieTexture;
	delete _pacmanTexture;
	
	delete _pacman;

	for (int iii = 0; iii < GHOSTCOUNT; iii++)
	{
		delete _ghosts[iii]->texture;
		delete _ghosts[iii]->sourceRect;
		delete _ghosts[iii]->posRect;
		delete _ghosts[iii];
	}

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

	delete _pop;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacmanTexture = new Texture2D();
	_pacmanTexture->Load("Textures/Pacman.tga", false);
	_pacman->Load(_pacmanTexture);

	// Load Munchies
	_munchieTexture = new Texture2D();
	_munchieTexture->Load("Textures/Cherry.png", false);
	for (int iii = 0; iii < _munchieCount; iii++)
	{
		LoadMunchie(_munchies[iii]);
	}

	//load ghost
	Texture2D* ghostTex = new Texture2D();
	ghostTex->Load("Textures/ghost.png", false);
	for (int iii = 0; iii < GHOSTCOUNT; iii++)
	{
		_ghosts[iii]->texture = ghostTex;
		_ghosts[iii]->posRect = new Rect(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight(), 20, 20);
		_ghosts[iii]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
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

	//load sounds
	_pop = new SoundEffect();
	_pop->Load("Sounds/pop.wav");
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

		if (!_pacman->IsDead())
		{
			_pacman->Update(elapsedTime);
		}

		UpdateMunchies(elapsedTime);

		CheckViewportCollision();

		CheckMunchieCollisions();

		UpdateGhosts(elapsedTime);

		CheckGhostCollision();
		
	}
}

void Pacman::Draw(int elapsedTime)
{
	//stream << "Munchie X: " << _munchie->position->X << " Y: " << _munchie->position->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing

	

	for (int iii = 0; iii < _munchieCount; iii++)
	{
		if (!_munchies[iii].eaten)
			SpriteBatch::Draw(_munchies[iii].texture, _munchies[iii].posRect, _munchies[iii].rect);
	}

	for (int iii = 0; iii < GHOSTCOUNT; iii++)
	{
		SpriteBatch::Draw(_ghosts[iii]->texture, _ghosts[iii]->posRect, _ghosts[iii]->sourceRect);
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
	
	_pacman->Draw();

	if (_pacman->IsDead())
	{
		std::stringstream menuStream;
		menuStream << "GAME OVER!";
		SpriteBatch::Draw(_gameOverMenu->background, _gameOverMenu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _gameOverMenu->stringPosition,
			Color::Red);
	}
	SpriteBatch::EndDraw(); // Ends Drawing
}

void Pacman::Input(int elapsedTime, Input::KeyboardState & state, const Input::MouseState & mouseState)
{

	/*if (mouseState.LeftButton == Input::ButtonState::PRESSED)
	{
		_munchie->position->Y = mouseState->Y - (_munchie->rect->Height / 2);
		_munchie->position->X = mouseState->X - (_munchie->rect->Width / 2);
	}*/

	if (state.IsKeyDown(Input::Keys::R) && !_rKeyDown)
	{
		for (int iii = 0; iii < _munchieCount; iii++)
		{
			_munchies[iii].posRect->X = rand() % (Graphics::GetViewportWidth() - _munchies[iii].rect->Width);
			_munchies[iii].posRect->Y = rand() % (Graphics::GetViewportHeight() - _munchies[iii].rect->Height);
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

void Pacman::CheckViewportCollision()
{
	for (int iii = 0; iii < _munchieCount; iii++)
	{
		if (_munchies[iii].posRect->X > Graphics::GetViewportWidth() - _munchies[iii].rect->X)
			_munchies[iii].posRect->X = 0;

		if (_munchies[iii].posRect->X < 0)
			_munchies[iii].posRect->X = Graphics::GetViewportWidth() - _munchies[iii].rect->Width;

		if (_munchies[iii].posRect->Y > Graphics::GetViewportHeight() - _munchies[iii].rect->Height)
			_munchies[iii].posRect->Y = 0;

		if (_munchies[iii].posRect->Y < 0)
			_munchies[iii].posRect->Y = Graphics::GetViewportHeight() - _munchies[iii].rect->Height;
	}

}

void Pacman::UpdateMunchies(int elapsedTime)
{
	if (_munchieCount == _munchiesEaten)
	{
		for (int iii = 0; iii < _munchieCount; iii++)
		{
			DeleteMunchie(&_munchies[iii]);
		}
		delete[] _munchies;

		_munchieCount = _munchieCount * _cMunchieMutiplier;
		_munchiesEaten = 0;

		_munchies = new Enemy[_munchieCount];

		for (int iii = 0; iii < _munchieCount; iii++)
		{
			LoadMunchie(_munchies[iii]);
		}
	}

	for (int iii = 0; iii < _munchieCount; iii++)
	{
		UpdateMunchie(_munchies[iii], elapsedTime);
	}
}

void Pacman::UpdateMunchie(Enemy& munchie, int elapsedTime)
{
	munchie.currentFrameTime += elapsedTime;
	if (munchie.currentFrameTime > _cMunchieFrameTime)
	{
		munchie.frameCount++;

		if (munchie.frameCount >= 2)
			munchie.frameCount = 0;
		
		munchie.currentFrameTime = 0;
	}

	munchie.rect->X = munchie.rect->Width * munchie.frameCount;

}

void Pacman::CheckMunchieCollisions()
{
	for (int iii = 0; iii < _munchieCount; iii++)
	{
		if (!_munchies[iii].eaten)
		{
			if (_pacman->GetBoundingCircle().Intersects(*_munchies[iii].posRect))
			{
				Audio::Play(_pop);
				_munchies[iii].eaten = true;
				_munchiesEaten++;
			}
		}
	}
}

void Pacman::LoadMunchie(Enemy& munchie)
{
	munchie.texture = _munchieTexture;
	munchie.frameCount = rand() % 1;
	munchie.currentFrameTime = (rand() % 500) + 50;
	munchie.eaten = false;
	munchie.rect = new Rect(0.0f, 0.0f, 32, 32);
	munchie.posRect = new Rect(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight(), 32, 32);
}

void Pacman::DeleteMunchie(Enemy* munchie)
{
	delete munchie->rect;
	munchie->rect = nullptr;

	delete munchie->posRect;
	munchie->posRect = nullptr;

	munchie->texture = nullptr;
}

void Pacman::UpdateGhosts(int elapsedTime)
{
	for (int iii = 0; iii < GHOSTCOUNT; iii++)
	{
		int startDirection = _ghosts[iii]->direction;

		if (_ghosts[iii]->direction == 0)
		{
			//move right
			_ghosts[iii]->posRect->X += _ghosts[iii]->speed * elapsedTime;
		}
		else if (_ghosts[iii]->direction == 2)
		{
			//move left
			_ghosts[iii]->posRect->X -= _ghosts[iii]->speed * elapsedTime;
		}
		else if (_ghosts[iii]->direction == 1)
		{
			//move down
			_ghosts[iii]->posRect->Y += _ghosts[iii]->speed * elapsedTime;
		}
		else if (_ghosts[iii]->direction == 3)
		{
			//move up
			_ghosts[iii]->posRect->Y -= _ghosts[iii]->speed * elapsedTime;
		}

		if (_ghosts[iii]->behaviour == EnemyBehaviour::MOVE_RANDOM)
		{
			_ghosts[iii]->speed = 0.1f;
			_ghosts[iii]->sourceRect->X = 0;

			if (_ghosts[iii]->posRect->X + _ghosts[iii]->posRect->Width >= Graphics::GetViewportWidth())
			{
				_ghosts[iii]->direction = 2;
			}
			else if (_ghosts[iii]->posRect->X <= 0)
			{
				_ghosts[iii]->direction = 0;
			}
			else if (_ghosts[iii]->posRect->Y + _ghosts[iii]->posRect->Height >= Graphics::GetViewportHeight())
			{
				_ghosts[iii]->direction = 3;
			}
			else if (_ghosts[iii]->posRect->Y <= 0)
			{
				_ghosts[iii]->direction = 1;
			}

			if (_ghosts[iii]->currentDirectionTime >= _ghosts[iii]->directionTime)
			{
				_ghosts[iii]->direction = rand() % 4;
			}

			if (Vector2::Distance(_pacman->GetPosition().Center(), _ghosts[iii]->posRect->Center()) < _cEnemyDistanceStartChase)
			{
				_ghosts[iii]->previousBehaviour = _ghosts[iii]->behaviour;
				_ghosts[iii]->behaviour = EnemyBehaviour::CHASE;
				//
			}
		}
		else if (_ghosts[iii]->behaviour == EnemyBehaviour::CHASE)
		{
			if (!_pacman->IsDead())
			{
				_ghosts[iii]->speed = 0.15f;
				_ghosts[iii]->sourceRect->X = 40.0f;
				if (_ghosts[iii]->currentDirectionTime > _cEnemyMinDirectionTime)
				{
					if (abs(_ghosts[iii]->posRect->X - _pacman->GetPosition().X) > abs(_ghosts[iii]->posRect->Y - _pacman->GetPosition().Y))
					{
						if (_ghosts[iii]->posRect->X > _pacman->GetPosition().X)
						{
							_ghosts[iii]->direction = 2;
						}
						else
						{
							_ghosts[iii]->direction = 0;
						}
					}
					else
					{
						if (_ghosts[iii]->posRect->Y > _pacman->GetPosition().Y)
						{
							_ghosts[iii]->direction = 3;
						}
						else
						{
							_ghosts[iii]->direction = 1;
						}
					}
				}
				if (Vector2::Distance(_pacman->GetPosition().Center(), _ghosts[iii]->posRect->Center()) > _cEnemyDistanceStopChase)
				{
					_ghosts[iii]->behaviour = _ghosts[iii]->previousBehaviour;
					_ghosts[iii]->previousBehaviour = EnemyBehaviour::CHASE;
				}

			}
			else
			{
				_ghosts[iii]->behaviour = _ghosts[iii]->previousBehaviour;
				_ghosts[iii]->previousBehaviour = EnemyBehaviour::CHASE;
			}
		}
		else if (_ghosts[iii]->behaviour == EnemyBehaviour::PATROL)
		{
			_ghosts[iii]->speed = 0.1f;
			_ghosts[iii]->sourceRect->X = 20.0f;
			if (_ghosts[iii]->direction == 0)
			{
				if (_ghosts[iii]->posRect->X + _ghosts[iii]->posRect->Width >= Graphics::GetViewportWidth())
				{
					_ghosts[iii]->direction = 3;
				}
			}
			if (_ghosts[iii]->direction == 2)
			{
				if (_ghosts[iii]->posRect->X <= 0)
				{
					_ghosts[iii]->direction = 1;
				}
			}
			if (_ghosts[iii]->direction == 1)
			{
				if (_ghosts[iii]->posRect->Y + _ghosts[iii]->posRect->Height >= Graphics::GetViewportHeight())
				{
					_ghosts[iii]->direction = 0;
				}
			}
			if (_ghosts[iii]->direction == 3)
			{
				if (_ghosts[iii]->posRect->Y <= 0)
				{
					_ghosts[iii]->direction = 2;
				}
			}
			if (Vector2::Distance(_pacman->GetPosition().Center(), _ghosts[iii]->posRect->Center()) < _cEnemyDistanceStartChase)
			{
				_ghosts[iii]->previousBehaviour = _ghosts[iii]->behaviour;
				_ghosts[iii]->behaviour = EnemyBehaviour::CHASE;
			}
		}

		if (startDirection != _ghosts[iii]->direction)
		{
			_ghosts[iii]->currentDirectionTime = 0;
			_ghosts[iii]->directionTime = (rand() % 1500) + 1000;
		}
		else
		{
			_ghosts[iii]->currentDirectionTime += elapsedTime;
		}

		_ghosts[iii]->sourceRect->Y = _ghosts[iii]->sourceRect->Height * _ghosts[iii]->direction;
	}
}

void Pacman::CheckGhostCollision()
{
	for (int iii = 0; iii < GHOSTCOUNT; iii++)
	{
		if (_pacman->GetBoundingCircle().Intersects(*_ghosts[iii]->posRect))
			_pacman->Kill();
	}
}

Player& Pacman::GetPlayer()
{
	return *_pacman;
}