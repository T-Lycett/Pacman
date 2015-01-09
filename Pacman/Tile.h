#pragma once
#include "GameObject.h"
class Tile :
	public GameObject
{
private:
	bool _collidable;
	bool _blocksSight;
	bool _revealed;
	bool _animated;

	int _frame;
	int _currentFrameTime;
	const int _cFrameTime;

	Texture2D* _fogOfWarTexture;
public:
	static const int SIZE = 32;

	Tile();
	~Tile();

	void Load(const char* textureFile, Texture2D* fogOfWarTexture, int tileX, int tileY, bool collidable = false, bool blocksSight = false, bool animated = false);

	void Update(int elapsedTime);
	void Draw();

	const Rect& GetBoundingRect() { return *_posRect; }

	bool IsCollidable() { return _collidable; }
	bool BlocksSight() { return _blocksSight; }

	void Reveal() { _revealed = true; }
};