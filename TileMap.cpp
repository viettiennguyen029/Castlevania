#include "TileMap.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

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
	float x =0;
	float y = 0;
		
	// draw tiles within the viewport only
	int colCamLeft = x/tileWidth;
	int colCamRight = colCamLeft + SCREEN_WIDTH / tileWidth+SCREEN_WIDTH/2;

	int rowCamTop = y / tileHeight;
	int rowCamBottom = rowCamTop + 180 / tileHeight;

	for (int j = colCamLeft; j <= colCamRight; j++) 
	{
		for (int i = rowCamTop; i < rowCamBottom; i++)
		{
			float pos_x = (j - colCamLeft) * tileWidth+ translate_x;
			float pos_y = (i - rowCamTop) * tileHeight+translate_y+22;

			RECT rectTile;
			int index = tiledMap[i][j];

			rectTile.left = (index % mapColumns) * tileWidth;
			rectTile.top = (index / mapColumns) * tileHeight;
			rectTile.right = rectTile.left + tileWidth;
			rectTile.bottom = rectTile.top + tileHeight;

			 CGame::GetInstance()->Draw(pos_x, pos_y, -1,CTextures::GetInstance()->Get(MAP_SCENCE_1), rectTile.left, rectTile.top, rectTile.right, rectTile.bottom);
			
		}
	}

	
}


