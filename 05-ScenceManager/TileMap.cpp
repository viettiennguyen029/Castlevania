#include "TileMap.h"
#include <stdio.h>
#include <stdlib.h>

CTileMap::CTileMap(LPCWSTR picturePath, int id, int translate_y, int translate_x)
{
	id = id;

	translate_y = translate_y;
	translate_x = translate_x;

	textures->Add(id, picturePath, D3DCOLOR_XRGB(0, 0, 0));

}

void CTileMap::LoadMap(const char *filePath)
{
	FILE* pFile;
	fopen_s(&pFile,filePath, "r");

	// Reading first line
	fscanf_s(pFile, "%d %d %d %d", &mapRows, &mapColumns, &tileWidth, &tileHeight);

	// Then cotinue parsing matrix
	for (int i = 0; i < mapRows; i++)
	{
		for (int j = 0; j < mapColumns; j++)
		{
			fscanf_s(pFile, "%d", &tiledMap[i][j]);
		}
	}


	fclose(pFile);
}

void CTileMap::DrawMap()
{
	float cam_x = CGame::GetInstance()->GetCamPos().x;
	float cam_y = CGame::GetInstance()->GetCamPos().y;

	float remain_x = fmod(cam_x, tileWidth);
	float remain_y = fmod(cam_y, tileHeight);

	// draw tiles within the viewport only
	int colCamLeft = cam_x/tileWidth;
	int colCamRight = colCamLeft + 320 / tileWidth;

	int rowCamTop = cam_y / tileHeight;
	int rowCamBottom = rowCamTop + 240/ tileHeight;

	for (int j = colCamLeft; j <= colCamRight; j++) 
	{
		for (int i = rowCamTop; i < rowCamBottom; i++)
		{
			float pos_x = (j - colCamLeft) * tileWidth- remain_x+ translate_x;
			float pos_y = (i - rowCamTop) * tileHeight-remain_y+translate_y+20;

			RECT rectTile;
			int index = tiledMap[i][j];

			rectTile.left = (index % mapColumns) * tileWidth;
			rectTile.top = (index / mapColumns) * tileHeight;
			rectTile.right = rectTile.left + tileWidth;
			rectTile.bottom = rectTile.top + tileHeight;

			CGame::GetInstance()->Draw(pos_x, pos_y, 1,CTextures::GetInstance()->Get(MAP_SCENCE_1), rectTile.left, rectTile.top, rectTile.right, rectTile.bottom);
		}
	}
}
