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


bool Map::InLineOfSight(Vector2& pos1, Vector2& pos2)
{
	int x1, y1, x2, y2;
	if (pos1.X < pos2.X)
	{
		x1 = pos1.X;
		y1 = pos1.Y;
		x2 = pos2.X;
		y2 = pos2.Y;
	}
	else
	{
		x1 = pos2.X;
		y1 = pos2.Y;
		x2 = pos1.X;
		y2 = pos1.Y;
	}

	if (x1 != x2)
	{
		float m = (float) (y2 - y1) / (x2 - x1);
		float b = y1 - (m * x1);
		float x = x1 + (Tile::SIZE - (x1 % Tile::SIZE)) + 1;
		float y = (m * x) + b;

		while (x < x2 - (x2 % Tile::SIZE))
		{
			if (GetTileAtPosition(x, y).BlocksSight())
				return false;

			x += Tile::SIZE;
			y = (m * x) + b;
		}
	}

	if (pos1.Y < pos2.Y)
	{
		x1 = pos1.X;
		y1 = pos1.Y;
		x2 = pos2.X;
		y2 = pos2.Y;
	}
	else
	{
		x1 = pos2.X;
		y1 = pos2.Y;
		x2 = pos1.X;
		y2 = pos1.Y;
	}

	if (y1 != y2)
	{
		float m = (float) (x2 - x1) / (y2 - y1);
		float b = x1 - (m * y1);
		float y = y1 + (Tile::SIZE - (y1 % Tile::SIZE)) + 1;
		float x = (m * y) + b;

		while (y < y2 - (y2 % Tile::SIZE))
		{
			if (GetTileAtPosition(x, y).BlocksSight())
				return false;

			y += Tile::SIZE;
			x = (m * y) + b;
		}
	}
	return true;
}