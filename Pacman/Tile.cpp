#include "Tile.h"


Tile::Tile() : _cFrameTime(500)
{
	_revealed = false;

	_frame = 0;
	_currentFrameTime = 0;
}


Tile::~Tile()
{
	delete _texture;
}


void Tile::Load(const char* textureFile, Texture2D* fogOfWarTexture, int tileX, int tileY, bool collidable, bool blocksSight, bool animated)
{
	_texture = new Texture2D();
	_texture->Load(textureFile, false);
	_fogOfWarTexture = fogOfWarTexture;
	_posRect = new Rect(tileX * Tile::SIZE, tileY * Tile::SIZE, Tile::SIZE, Tile::SIZE);
	_sourceRect = new Rect(0.0f, 0.0f, Tile::SIZE, Tile::SIZE);
	_collidable = collidable;
	_blocksSight = blocksSight;
	_animated = animated;
}


void Tile::Update(int elapsedTime)
{
	if (_animated && _draw)
	{
		_currentFrameTime += elapsedTime;

		if (_currentFrameTime > _cFrameTime)
		{
			_frame++;

			if (_frame >= 2)
				_frame = 0;

			_currentFrameTime = 0;
		}

		_sourceRect->X = _sourceRect->Width * _frame;
	}
}


void Tile::Draw()
{
	if (_draw == false && _revealed == true)
	{
		SpriteBatch::Draw(_texture, _posRect, _sourceRect);
		SpriteBatch::Draw(_fogOfWarTexture, _posRect, _sourceRect);
		return;
	}
	GameObject::Draw();
}