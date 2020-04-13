#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Sprites.h"
#include "Textures.h"

using namespace std;

class CTileMap
{
	CSprites* sprites = CSprites::GetInstance();
	vector<vector<LPSPRITE>> tiledMap;

	int ID;

	int mapWidth;
	int mapHeight;

	int rows;
	int columns;

	int tileWidth;
	int tileHeight;

	LPCWSTR picPath;
	LPCWSTR dataPath;		
	// tile[50][100];

public: 
	CTileMap(int ID, LPCWSTR picPath, LPCWSTR dataPath, int mapWidth, int mapHeight, int tileWidth = 32, int tileHeight = 32);
	void LoadMap();
	void LoadResources();
	void DrawMap(D3DXVECTOR2 camPosition);
};
typedef CTileMap* LPTILEMAP;


/*
Manage TileMaps database
*/
class CTileMaps
{
	static CTileMaps* _instance;
	unordered_map<int, LPTILEMAP> tilemaps;

public:
	 void Add(int ID, LPCWSTR picPath, LPCWSTR dataPath, int mapWidth, int mapHeight, int tileWidth = 32, int tileHeight = 32);
	LPTILEMAP Get(int ID) { return tilemaps[ID]; }

	static CTileMaps* GetInstance();
};



