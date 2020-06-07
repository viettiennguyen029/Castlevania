#pragma once
#include "GameObject.h"
#include "WallPieces.h"
#include "Items.h"

#define BREAK_WALL_WIDTH	16
#define BREAK_WALL_HEIGHT 16

#define WALL_STATE_NORMAL 0
#define WALL_STATE_BREAK	1

#define WALLPIECES_TIME		2000

class CBreakWall: public CGameObject
{
public:
	CBreakWall(float x, float y);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL) {};
	void Destroy();
	void SetState(int state);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};
typedef CBreakWall* LPBREAKWALL;

