#include "Map.h"
#include "tilemap.h"
#include "Utils.h"

CMaps* CMaps::_instance = NULL;

CMap::CMap(int ID, LPCWSTR filePath_tex, LPCWSTR filePath_data, int map_width, int map_height)
{
	this->ID = ID;

	this->filePath_tex = filePath_tex;
	this->filePath_data = filePath_data;

	this->map_Width = map_width;
	this->map_Height = map_height;

	nums_row = map_Height / TILE_HEIGHT;
	nums_col = map_Width / TILE_WIDTH;

	LoadResources();
	LoadMap();
}

void CMap::LoadResources()
{
	CTextures* texture = CTextures::GetInstance();

	texture->Add(ID, filePath_tex, TILEMAP_TRANSPARENT_COLOR);

	LPDIRECT3DTEXTURE9 texTileMap = texture->Get(ID);

	// lấy thông tin về kích thước của texture lưu các block tiles (filePath_tex)
	D3DSURFACE_DESC surfaceDesc;
	int level = 0;
	texTileMap->GetLevelDesc(level, &surfaceDesc);

	// tính toán số hàng, số cột cần thiết để đọc các ô tile từ file
	int nums_rowToRead = surfaceDesc.Height / TILE_HEIGHT;
	int nums_colToRead = surfaceDesc.Width / TILE_WIDTH;

	// thực hiện lưu danh sách các tile vô sprites theo thứ tự id_sprite
	int id_sprite = 1;

	for (UINT i = 0; i < nums_rowToRead; i++)
	{
		for (UINT j = 0; j < nums_colToRead; j++)
		{
			int idTile = ID * 10 + id_sprite;
			sprites->Add(idTile, TILE_WIDTH * j, TILE_HEIGHT * i, TILE_WIDTH * (j + 1), TILE_HEIGHT * (i + 1), texTileMap);
			id_sprite = id_sprite + 1;
		}
	}
}

void CMap::LoadMap()
{
	fstream fs;
	fs.open(filePath_data, ios::in);

	if (fs.fail())
	{
		DebugOut(L"[ERROR] CMap::Load_MapData failed: ID=%d", ID);
		fs.close();
		return;
	}

	string line;

	while (!fs.eof())
	{
		getline(fs, line);

		// Lưu sprite tile vào vector tilemap
		vector<LPSPRITE> spriteline;
		stringstream ss(line);
		int n;

		while (ss >> n)
		{
			int idTile = ID * 1000 + n;
			spriteline.push_back(sprites->Get(idTile));
		}

		tilemap.push_back(spriteline);
	}

	fs.close();
}

void CMap::Draw(D3DXVECTOR2 camPosition)
{
	int start_col_to_draw = (int)camPosition.x / 32;
	int end_col_to_draw = start_col_to_draw + SCREEN_WIDTH / 32;

	for (UINT i = 0; i < nums_row; i++)
	{
		for (UINT j = start_col_to_draw; j <= end_col_to_draw; j++)
		{
			// +camPosition.x để luôn giữ camera ở chính giữa, vì trong hàm Game::Draw() có trừ cho camPosition.x làm các object đều di chuyển theo
			// +(int)camPosition.x % 32 để giữ cho camera chuyển động mượt
			float x = TILE_WIDTH * (j - start_col_to_draw) + camPosition.x - (int)camPosition.x % 32;
			float y = TILE_HEIGHT * i + 80;

			tilemap[i][j]->Draw(x, y, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

void CMaps::Add(LPCWSTR filePath_data, LPCWSTR filePath_tex, int ID)
{
	LPTILEMAP tilemap = new CMap(ID, filePath_tex, filePath_data, MAP_1_WITDH, MAP_1_HEIGHT);
	tilemaps[ID] = tilemap;
}

CMaps* CMaps::GetInstance()
{
	if (_instance == NULL) _instance = new CMaps();
	return _instance;
}
