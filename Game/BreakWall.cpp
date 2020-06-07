#include "BreakWall.h"

CBreakWall::CBreakWall(float x, float y) : CGameObject()
{
	this->x = x;
	this->y = y;
}

//void CBreakWall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
//{
//	if (state == WALL_STATE_BREAK)
//	{
//		CWallPieces::GetInstance()->DropPiece(this->x, this->y);
//		//CItems::GetInstance()->CheckAndDrop(this);
//		this->visible = false;
//	}
//
//}

void CBreakWall::Render()
{	
	animation_set->at(state)->Render(x, y, -1);
}

void CBreakWall::Destroy()
{
	CWallPieces::GetInstance()->DropPiece(this->x, this->y);
	this->visible = false;
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
