#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "Sprites.h"
#include "Utils.h"
#include "Textures.h"

#define TILEMAP_TRANSPARENT_COLOR		D3DCOLOR_XRGB(5,5,5)

#define MAP_1_WITDH			1536
#define MAP_1_HEIGHT		320

#define TILE_WIDTH		32
#define TILE_HEIGHT		32

#define MAP_1_MIN_COL	0
#define MAP_1_MAX_COL	47

// Scene
#define SCENE_1			1000
using namespace std;

class CMap
{
	CSprites* sprites = CSprites::GetInstance();
	vector<vector<LPSPRITE>> tilemap;

	int map_Width;					// chiều rộng của map
	int map_Height;					// chiều cao của map

	int nums_row;					// số hàng của map
	int nums_col;					// số cột của map

	LPCWSTR filePath_tex;			// filepath texture (chứa các ô tile)
	LPCWSTR filePath_data;			// filepath data (chứa ma trận map)

	int ID;							// ID của tile map, dùng để tạo class quản lí CMaps sau này.

public:
	int index = 0;							 // index hiện tại của min_max_col_to_draw

	CMap() {}
	CMap(int ID, LPCWSTR filePath_tex, LPCWSTR filePath_data, int map_width, int map_height);

	void LoadResources();			// load tất cả các ô tile lưu vào sprites
	void LoadMap();					// load ma trận map 
	void Draw(D3DXVECTOR2 camPosition);	// vẽ tất cả các tile nằm trong camera

	int GetMapWidth() { return map_Width; }
};

typedef CMap* LPTILEMAP;

/*
	Class quản lí danh sách các Tilemap
*/
class CMaps
{
	static CMaps* _instance;
	unordered_map<int, LPTILEMAP> tilemaps;

public:
	void Add(LPCWSTR filePath_data, LPCWSTR filePath_tex, int ID);
	LPTILEMAP Get(int ID) { return tilemaps[ID]; }

	static CMaps* GetInstance();
};
