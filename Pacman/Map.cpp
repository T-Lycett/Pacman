#include "Map.h"
#include <fstream>
#include <string>

Map::Map()
{
	_floorTexture = new Texture2D();
	_wallTexture = new Texture2D();
	_fogOfWarTexture = new Texture2D();
}


Map::~Map()
{
	for (int iii = 0; iii < _height; iii++)
	{
		delete[] _tiles[iii];
	}
	delete[] _tiles;

	delete _floorTexture;
	delete _wallTexture;
	delete _fogOfWarTexture;
}


void Map::Load(const char* filename, int width, int height)
{
	_floorTexture->Load("Textures/floor.png", false);
	_wallTexture->Load("Textures/wall.png", false);
	_fogOfWarTexture->Load("Textures/Transparency.png", false);
	
	_tiles = new Tile*[height];
	for (int iii = 0; iii < height; iii++)
	{
		_tiles[iii] = new Tile[width];
	}
	_height = height;
	_width = width;

	ifstream mapFile;
	mapFile.open(filename);
	string row;
	for (int iii = 0; iii < _height; iii++)
	{
		getline(mapFile, row);
		for (int jjj = 0; jjj < _width; jjj++)
		{
			LoadTile(row[jjj], jjj, iii);
		}
	}
	mapFile.close();
}


void Map::LoadTile(char type, int X, int Y)
{
	switch (type)
	{
	case 'f':
		_tiles[Y][X].Load(_floorTexture, _fogOfWarTexture, X, Y);
		break;
	case 'w':
		_tiles[Y][X].Load(_wallTexture, _fogOfWarTexture, X, Y, true, true);
	}
}


void Map::Draw()
{
	DrawTiles();
}


void Map::DrawTiles()
{
	for (int iii = 0; iii < _height; iii++)
	{
		for (int jjj = 0; jjj < _width; jjj++)
		{
			_tiles[iii][jjj].Draw();
		}
	}
}


Tile& Map::GetTile(int X, int Y)
{
	return _tiles[Y][X];
}


Tile& Map::GetTileAtPosition(float X, float Y)
{
	if (Y < _height * Tile::SIZE && X < _width * Tile::SIZE)
		return _tiles[(int)(Y / Tile::SIZE)][(int)(X / Tile::SIZE)];
}