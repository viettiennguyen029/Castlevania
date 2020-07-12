#include "Ghost.h"

void CGhost::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + GHOST_BBOX_WIDTH;
	bottom = top + GHOST_BBOX_HEIGHT;
}

void CGhost::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (start_untouchable != 0)
	{
		Untouchable();
	}

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

		x += min_tx * dx;
		y += min_ty * dy;		
	}
	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CGhost::Render()
{
	animation_set->at(state)->Render(x, y, nx);	
}

CGhost::CGhost(float x, float y)
{
	this->start_x = x;
	this->start_y = y;
	this->healthPoint = 2;
	SetState(GHOST_STATE_MOVING);
}

void CGhost::SetState(int state)
{
	CGameObject::SetState(state);
}
