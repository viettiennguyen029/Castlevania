#include "Black_Knight.h"

void CBlack_Knight::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x; 
	top = y;
	right = left + BLACK_KNIGHT_BBOX_WIDTH;
	bottom = left + BLACK_KNIGHT_BBOX_HEIGHT;
}

void CBlack_Knight::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;

	if (vx < 0 && x < 36) {
		nx = -nx;
		x = 36; vx = -vx;
	}

	if (vx > 0 && x > 122) {
		nx = -nx;
		x = 122; vx = -vx;
	}
}

void CBlack_Knight::Render()
{
	animation_set->at(state)->Render(x, y,nx);
}

CBlack_Knight::CBlack_Knight()
{
	SetState(BLACK_KNIGHT_STATE_WALKING);
}

void CBlack_Knight::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BLACK_KNIGHT_STATE_WALKING:
		vx = BLACK_KNIGHT_WALKING_SPEED;
		break;
	}

}
