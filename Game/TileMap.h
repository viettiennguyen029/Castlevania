#pragma once
#include "Utils.h"
#include "Sprites.h"
#include "Textures.h"
#include "Game.h"

#define MAP_SCENCE_1 -10

#define SCREEN_WIDTH 280
#define SCREEN_HEIGHT 180

using namespace std;

class CTileMap
{
	int ID;

	int tileWidth;
	int tileHeight;

	int translate_y, translate_x;

	int mapRows; // number of  rows in map matrix
	int mapColumns; // // number of  columns in map matrix

	int tiledMap[50][100]; // 2 dimensional array for parsing data from file
public:
	CGame* game = CGame::GetInstance();
	CTextures* textures = CTextures::GetInstance();

	CTileMap(LPCWSTR picturePath, int id, int translate_x, int translate_y);
	void LoadMap(const char* filePath);
	void DrawMap();
};