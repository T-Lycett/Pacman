#pragma once
#include "Tile.h"

class Map
{
private:
	Tile** _tiles;
	int _height, _width;

	Texture2D* _floorTexture;
	Texture2D* _wallTexture;
	Texture2D* _fogOfWarTexture;

	void LoadTile(char type, int x, int y);

	void DrawTiles();
public:
	Map();
	~Map();

	void Load(const char* filename, int width, int height);

	void Draw();

	Tile& GetTile(int X, int Y);
	Tile& GetTileAtPosition(float X, float Y);
};

