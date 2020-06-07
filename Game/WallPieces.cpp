#include "WallPieces.h"
#define WALLPIECES_GRAVITY	0.0005f

CWallPiece::CWallPiece()
{
	SetVisible(false);
}

void CWallPiece::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	CGameObject::Update(dt);
	vy += WALLPIECES_GRAVITY * dt;

	x += dx;
	y += dy;

	if (y >= 200) SetVisible(false);
}

void CWallPiece::Render()
{
	animation_set->at(state)->Render(x, y, -1);
}

void CWallPieces::AddPiece(CWallPiece* piece)
{
	piece->SetVisible(false);
	pieces.push_back(piece);
}

void CWallPieces::DropPiece(float x, float y)
{
	CWallPiece* piece = NULL;
	UINT i = 0;
	
	for (int j = 0; j < 4; j++) // Each group has 4 pieces
	{
		for (; i < pieces.size(); i++)
		{
			if (pieces[i]->isVisible() == false)
			{
				piece = pieces[i];
				i++;
				break;
			}
		}
		if (piece != NULL)
		{
			piece->SetPosition(x, y);
			piece->SetVisible(true);
			float vx = (float)(-100 + rand() % 200) / 1000;
			float vy = (float)(-100 + rand() % 200) / 1000;
			piece->SetSpeed(vx, -vy);
			
		}
	}
	
}

CWallPieces* CWallPieces::__instance = NULL;
CWallPieces* CWallPieces::GetInstance()
{
	if (__instance == NULL)
		__instance = new CWallPieces();
	return __instance;
}
