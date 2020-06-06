#include "WallPieces.h"
#define WALLPIECES_GRAVITY	0.0005f

CWallPiece::CWallPiece()
{
}

void CWallPiece::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	CGameObject::Update(dt);
	vy += WALLPIECES_GRAVITY * dt;

	x += dx;
	y += dy;
}

void CWallPiece::Render()
{
	animation_set->at(state)->Render(x, y, -1);
}

CWallPieces* CWallPieces::__instance = NULL;
void CWallPieces::AddPiece(LPWALLPIECE piece)
{
	pieces.push_back(piece);

}



CWallPieces* CWallPieces::GetInstance()
{
	if (__instance == NULL)
		__instance = new CWallPieces();

	return __instance;
}
