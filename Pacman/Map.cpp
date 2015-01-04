#include "Map.h"
#include <fstream>
#include <string>

Map::Map()
{
	_floorTexture = new Texture2D();
	_wallTexture = new Texture2D();
	_fogOfWarTexture = new Texture2D();
	_loaded = false;
}


Map::~Map()
{
	DeleteTiles();

	delete _floorTexture;
	delete _wallTexture;
	delete _fogOfWarTexture;
}


void Map::Load(const char* filename, int width, int height)
{
	if (!_loaded)
	{
		_loaded = true;

		_floorTexture->Load("Textures/floor.png", false);
		_wallTexture->Load("Textures/wall.png", false);
		_fogOfWarTexture->Load("Textures/Transparency.png", false);

		_height = height;
		_width = width;

		LoadTiles(filename);
	}
	else
	{
		DeleteTiles();

		_height = height;
		_width = width;

		LoadTiles(filename);
	}
}


void Map::LoadTiles(const char* filename)
{
	_tiles = new Tile*[_height];
	for (int iii = 0; iii < _height; iii++)
	{
		_tiles[iii] = new Tile[_width];
	}

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


void Map::DeleteTiles()
{
	for (int iii = 0; iii < _height; iii++)
	{
		delete[] _tiles[iii];
	}
	delete[] _tiles;
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
		break;
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


bool Map::IsValidTile(int X, int Y)
{
	if (X >= 0 && Y >= 0 && X < _width && Y < _height)
	{
		return true;
	}
	return false;
}