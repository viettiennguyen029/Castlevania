#include "Items.h"

void CItems::Update(DWORD dt, vector<LPGAMEOBJECT>* Objects, vector<LPGAMEOBJECT>* coObjects)
{
	if (timeAppear == -1)
	{
		timeAppear = GetTickCount();
	}
	else
	{
		DWORD now = GetTickCount();

		if (now - timeAppear > ITEM_TIME_DESTROYED)
		{
			visible = false;
			return;
		}
	}

	CGameObject::Update(dt);

	// Check collision between item and ground (falling on ground)
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx, rdy;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny,rdx,rdy);

		y += min_ty * dy + ny * 0.1f;
		if (ny != 0) vy = 0;
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CItems::Render()
{
	// animations[state]->Render(-1, x, y);
	animation_set->at(state)->Render(x, y, nx);
}

void CItems::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	left = x;
	top = y;
	switch(state)
	{
	case BIG_HEART: 
		right = left + BIG_HEART_BBOX_WIDTH;
		bottom = top + BIG_HEART_BBOX_HEIGHT;
		break;
	case CHAIN: 
		right = left + CHAIN_BBOX_WIDTH;
		bottom = top + CHAIN_BBOX_HEIGHT;
		break;
	case DAGGER: 
		right = left + DAGGER_BBOX_WIDTH;
		bottom = top + DAGGER_BBOX_HEIGHT;
		break;
	default:
		break;
	 }
}

CItems::CItems() : CGameObject()
{
	vy = ITEM_FALLING_SPEED;
	timeAppear = -1;
}

void CItems::SetItem(int itemId)
{
	this-> state = itemId;
}