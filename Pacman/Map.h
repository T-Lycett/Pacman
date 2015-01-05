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

	bool _loaded;

	void LoadTiles(const char* filename);
	void LoadTile(char type, int x, int y);

	void DeleteTiles();

	void DrawTiles();
public:
	Map();
	~Map();

	void Load(const char* filename, int width, int height);

	void Draw();

	Tile& GetTile(int X, int Y);
	Tile& GetTileAtPosition(float X, float Y);
	bool IsValidTile(int X, int Y);

	bool InLineOfSight(Vector2& pos1, Vector2& pos2);

	int GetHeight() { return _height; }
	int GetWidth() { return _width; }
};

