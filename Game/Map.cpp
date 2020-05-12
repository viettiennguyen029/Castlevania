#include "Map.h"
#include "Utils.h"

CMaps* CMaps::__instance = NULL;

CMap::CMap(int ID, LPCWSTR texPath, LPCWSTR dataPath, int mapwidth, int mapheight)
{
	this->mapId = ID;

	this->mapWidth = mapwidth;
	this->mapHeight = mapheight;

	LoadResources(texPath);
	LoadData(dataPath);
}

void CMap::LoadResources(LPCWSTR texPath)
{
	CSprites* sprites = CSprites::GetInstance();
	CTextures* textures = CTextures::GetInstance();

	textures->Add(mapId, texPath, D3DCOLOR_XRGB(5, 5, 5));

	LPDIRECT3DTEXTURE9 texTileMap = textures->Get(mapId);

	D3DSURFACE_DESC surfaceDesc;
	int level = 0;
	texTileMap->GetLevelDesc(level, &surfaceDesc);

	int nums_rowToRead = surfaceDesc.Height / TILE_HEIGHT;
	int nums_colToRead = surfaceDesc.Width / TILE_WIDTH;

	int id_sprite = 0;
	for (UINT i = 0; i < nums_rowToRead; i++)
	{
		for (UINT j = 0; j < nums_colToRead; j++)
		{
			int idTile = mapId + id_sprite;
			sprites->Add(idTile, TILE_WIDTH * j, TILE_HEIGHT * i, TILE_WIDTH * (j + 1), TILE_HEIGHT * (i + 1), texTileMap);
			id_sprite = id_sprite + 1;
		}
	}
}

void CMap::LoadData(LPCWSTR dataPath)
{
	CSprites* sprites = CSprites::GetInstance();
	fstream fs;
	fs.open(dataPath, ios::in);

	if (fs.fail())
	{
		DebugOut(L"[ERROR] CMap::Load_MapData failed: ID=%d", mapId);
		fs.close();
		return;
	}

	string line;
	while (!fs.eof())
	{
		getline(fs, line);

		vector<LPSPRITE> spriteline;
		stringstream ss(line);
		int n;

		while (ss >> n)
		 {
			int idTile = mapId + n;
			spriteline.push_back(sprites->Get(idTile));
		 }
		tiledmap.push_back(spriteline);
	}

	fs.close();
}

void CMap::DrawMap(D3DXVECTOR3 camPosition)
{
	int startCol = (int)camPosition.x / 32;
	int endCol = startCol +280 / 32+1;
	int numOfRow = tiledmap.size();

	for (int i = 0; i < numOfRow; i++)
	{
		for (int j = startCol; j <= endCol; j++)
		{
			float x = TILE_WIDTH * (j - startCol) + camPosition.x-(int)camPosition.x % 32;
			float y = TILE_HEIGHT * i +22;

			tiledmap[i][j]->Draw(x, y, -1);
		}
	}
}

void CMaps::Add(int id, LPCWSTR texPath, LPCWSTR dataPath, int mapwidth, int mapheight)
{
	LPTILEMAP tilemap = new CMap(id, texPath, dataPath, mapwidth, mapheight);
	maps[id] = tilemap;
}

CMaps* CMaps::GetInstance()
{
	if (__instance == NULL) __instance = new CMaps();
	return __instance;
}