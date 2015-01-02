#include "Tile.h"


Tile::Tile()
{
	_revealed = false;
}


Tile::~Tile()
{
}


void Tile::Load(Texture2D* texture, Texture2D* fogOfWarTexture, int tileX, int tileY, bool collidable, bool blocksSight)
{
	_texture = texture;
	_fogOfWarTexture = fogOfWarTexture;
	_posRect = new Rect(tileX * Tile::SIZE, tileY * Tile::SIZE, Tile::SIZE, Tile::SIZE);
	_sourceRect = new Rect(0.0f, 0.0f, Tile::SIZE, Tile::SIZE);
	_collidable = collidable;
	_blocksSight = blocksSight;
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