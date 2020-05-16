#include "Tile.h"
CTile::CTile(int _x, int _y, int _left, int _top, int _right, int _bottom)
{
	this->x = _x;
	this->y = _y;

	this->left = _left;
	this->top = _top;
	this->right = _right;
	this->bottom = _bottom;	
}

void CTile::Render()
{
	int currentMapId = CGame::GetInstance()->GetSceneId();
	LPDIRECT3DTEXTURE9 texture = CTextures::GetInstance()->Get(currentMapId);
	CGame* game = CGame::GetInstance();
	game->Draw(x, y, -1, texture, left, top, right, bottom, 255);
}
