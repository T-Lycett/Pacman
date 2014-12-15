#pragma once
#include "GameObject.h"
class Tile :
	public GameObject
{
public:
	static const int HEIGHT = 32, WIDTH = 32;

	Tile();
	~Tile();

	void Load(Texture2D* texture, Vector2 pos);
};