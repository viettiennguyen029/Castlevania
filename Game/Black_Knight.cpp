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
	CGameObject::Update(dt);
	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		y += dy;
		x += dx;
	}
	else
	{
		float min_tx, min_ty, nx, ny;
		float rdx, rdy;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx ;
		y += min_ty * dy+ ny*0.4f;

	}


	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
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
		vy = 0;
		break;
	}

}
