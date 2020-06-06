#include "BreakWall.h"

CBreakWall::CBreakWall(float x, float y) : CGameObject()
{
	this->x = x;
	this->y = y;
}

void CBreakWall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (state == WALL_STATE_BREAK)
	{
		isBreakedToPieces = true;
		//pieces = new CWallPieces(x, y);
		//CWallPieces::GetInstance()->DropWall();
	}

	/*if (pieces != NULL)
		pieces->Update(dt);*/
}

void CBreakWall::Render()
{
	if (state == WALL_STATE_NORMAL)
		return;

	animation_set->at(state)->Render(x, y,-1);
	//RenderBoundingBox();

}

void CBreakWall::SetState(int state)
{
	CGameObject::SetState(state);
}

void CBreakWall::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + BREAK_WALL_WIDTH;
	b = t + BREAK_WALL_HEIGHT;
}
