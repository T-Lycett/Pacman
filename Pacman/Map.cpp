#include "Map.h"
#include <fstream>
#include <string>
#include <sstream>

Map::Map()
{
	_fogOfWarTexture = new Texture2D();
	_loaded = false;
}


Map::~Map()
{
	DeleteTiles();

	delete _fogOfWarTexture;
}


void Map::Load(const char* folder, int width, int height)
{
	if (!_loaded)
	{
		_loaded = true;

		_fogOfWarTexture->Load("Textures/Transparency.png", false);

		_height = height;
		_width = width;

		LoadTiles(folder);
	}
	else
	{
		DeleteTiles();

		_height = height;
		_width = width;

		LoadTiles(folder);
	}
}


void Map::LoadTiles(const char* folder)
{
	_tiles = new Tile*[_height];
	for (int iii = 0; iii < _height; iii++)
	{
		_tiles[iii] = new Tile[_width];
	}

	ifstream mapFile;
	stringstream filename;
	filename << folder << "tiles.txt";
	mapFile.open(filename.str().c_str());
	string row;

	for (int iii = 0; iii < _height; iii++)
	{
		getline(mapFile, row);
		for (int jjj = 0; jjj < _width; jjj++)
		{
			stringstream textureFilename;
			textureFilename << folder << row[jjj * 2] << ".png";
			LoadTile(textureFilename.str().c_str(), row[(jjj * 2) + 1], jjj, iii);
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


void Map::LoadTile(const char* textureFilename, char type, int X, int Y)
{
	switch (type)
	{
	case '0':
		_tiles[Y][X].Load(textureFilename, _fogOfWarTexture, X, Y);
		break;
	case '1':
		_tiles[Y][X].Load(textureFilename, _fogOfWarTexture, X, Y, false, false, true);
		break;
	case '2':
		_tiles[Y][X].Load(textureFilename, _fogOfWarTexture, X, Y, false, true, false);
		break;
	case '3':
		_tiles[Y][X].Load(textureFilename, _fogOfWarTexture, X, Y, false, true, true);
		break;
	case '4':
		_tiles[Y][X].Load(textureFilename, _fogOfWarTexture, X, Y, true, false, false);
		break;
	case '5':
		_tiles[Y][X].Load(textureFilename, _fogOfWarTexture, X, Y, true, false, true);
		break;
	case '6':
		_tiles[Y][X].Load(textureFilename, _fogOfWarTexture, X, Y, true, true, false);
		break;
	case '7':
		_tiles[Y][X].Load(textureFilename, _fogOfWarTexture, X, Y, true, true, true);
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