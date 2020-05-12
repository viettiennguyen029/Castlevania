#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "Textures.h"
#include "Sprites.h"

#define TILE_WIDTH	32
#define TILE_HEIGHT	32
using namespace std;

class CMap
{
	vector<vector<LPSPRITE>> tiledmap; 

	int mapWidth;
	int mapHeight;

	int mapId;
public:
	CMap(int ID, LPCWSTR texPath, LPCWSTR dataPath, int mapwidth, int mapheight);

	void LoadResources(LPCWSTR texPath);
	void LoadData(LPCWSTR dataPath);
	void DrawMap(D3DXVECTOR3 camPosition);

	int GetMapWidth() { return mapWidth; }
	int GetMapHeight() { return mapHeight; }
};
typedef CMap* LPTILEMAP;

/*
Manage tile maps database
*/
class CMaps
{
	static CMaps* __instance;
	unordered_map<int, LPTILEMAP> maps;

public:
	void Add(int id, LPCWSTR texPath, LPCWSTR dataPath, int mapwidth, int mapheight);
	LPTILEMAP Get(int ID) { return maps[ID]; }
	static CMaps* GetInstance();
};
