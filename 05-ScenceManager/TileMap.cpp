#include "TileMap.h"

CTileMap::CTileMap(int ID, LPCWSTR picPath, LPCWSTR dataPath, int mapWidth, int mapHeight, int tileWidth, int tileHeight)
{
	this->ID = ID;

	this->picPath = picPath;
	this->dataPath = dataPath;

	this->mapWidth = mapWidth;
	this->mapHeight = mapHeight;

	this->tileWidth = tileWidth;
	this->tileHeight = tileHeight;

	rows = mapHeight / tileHeight;
	columns = mapWidth / tileWidth;

	LoadResources();
	LoadMap();
	//DrawMap(D3DXVECTOR2 camPosition);
}

void CTileMap::LoadResources()
{
	CTextures* texture = CTextures::GetInstance();
	texture->Add(ID, picPath, D3DCOLOR_XRGB(5, 5, 5));

	LPDIRECT3DTEXTURE9 texTileMap = texture->Get(ID);

	// lấy thông tin về kích thước của texture lưu các block tiles (picPath)
	D3DSURFACE_DESC surfaceDesc;
	int level = 0;
	texTileMap->GetLevelDesc(level, &surfaceDesc);

	// tính toán số hàng, số cột cần thiết để đọc các ô tile từ file
	int maxRow = surfaceDesc.Height / tileHeight;
	int maxColumn = surfaceDesc.Width / tileWidth;

	// thực hiện lưu danh sách các tile vô sprites theo thứ tự id_sprite
	int id_sprite = 1;

	for (UINT i = 0; i < maxRow; i++)
	{
		for (UINT j = 0; j < maxColumn; j++)
		{
			int tiledID = id_sprite;
			sprites->Add(tiledID, tileWidth * j, tileHeight * i, tileWidth * (j + 1), tileHeight * (i + 1), texTileMap);
			id_sprite = id_sprite + 1;
		}
	}
}

void CTileMap::LoadMap()
{
	fstream fs;
	fs.open(dataPath,ios::in);

	if (fs.fail())
	{
		DebugOut(L"[ERROR] TileMap::Load_MapData failed: ID=%d", ID);
		fs.close();
		return;
	}
	
	string line;

	while (!fs.eof())
	{
		getline(fs, line);

		// Push back sprtie tile to vector tilemap
		vector<LPSPRITE> spriteLine;
		stringstream ss(line);
		int n;

		// Continue here
		while (ss>>n)
		{
			int tiledID = n;
			spriteLine.push_back(sprites->Get(tiledID));

		}

		tiledMap.push_back(spriteLine);
	}
	fs.close();
}



void CTileMap::DrawMap(D3DXVECTOR2 camPosition)
{
	//int colCam = CGame::GetInstance()->GetCamPos().x / tileWidth;
	//int rowCam = CGame::GetInstance()->GetCamPos().y /tileHeight;


	//int x = (int)CGame::GetInstance()->GetCamPos().x % tileWidth;
	//int y = (int)CGame::GetInstance()->GetCamPos().y % tileHeight;

	int start_col_to_draw = (int)CGame::GetInstance()->GetCamPos().x /32 ;
	int end_col_to_draw = start_col_to_draw + 312 / 32;

	for (UINT i = 0; i < rows; i++)
	{
		for (UINT j = start_col_to_draw; j <= end_col_to_draw; j++)
		{
			// +camPosition.x để luôn giữ camera ở chính giữa, vì trong hàm Game::Draw() có trừ cho camPosition.x làm các object đều di chuyển theo
			// +(int)camPosition.x % 32 để giữ cho camera chuyển động mượt
			float x = tileWidth * (j - start_col_to_draw) + camPosition.x - (int)camPosition.x % 32;
			float y = tileHeight * i + 80;
			
			tiledMap[i][j]->Draw( x, y);
		}
	}


}


CTileMaps* CTileMaps::_instance = NULL;

CTileMaps* CTileMaps::GetInstance()
{
	if (_instance == NULL) _instance = new CTileMaps();
	return _instance;
}

void CTileMaps::Add(int ID, LPCWSTR picPath, LPCWSTR dataPath, int mapWidth, int mapHeight, int tileWidth, int tileHeight)
{
	LPTILEMAP tilemap = new CTileMap(ID, picPath, dataPath, mapWidth, mapHeight, tileWidth, tileHeight);
	tilemaps[ID] = tilemap;
}


