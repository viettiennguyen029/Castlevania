#include "Ghost.h"

void CGhost::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + GHOST_BBOX_WIDTH;
	bottom = top + GHOST_BBOX_HEIGHT;
}

void CGhost::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving)
{
	if (stopMoving)
	{
		stop = true;
		return;
	}
	else
	{
		stop = false;
	}

	CGameObject::Update(dt);
	vy = 0;
	vx = (nx > 0) ? GHOST_MOVING_SPEED : -GHOST_MOVING_SPEED;

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
	if (stop)
	{
		int currentFrame = animation_set->at(0)->GetCurrentFrame();
		animation_set->at(0)->SetCurrentFrame(currentFrame);
		animation_set->at(0)->RenderByFrame(currentFrame, nx, x, y);
	}
	else
		animation_set->at(0)->Render(x, y, nx);
}

CGhost::CGhost()
{
	this->healthPoint = 2;
	SetState(GHOST_STATE_MOVING);
	this->nx = -1;
}

void CGhost::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GHOST_STATE_MOVING:
	{
		vy = 0;
		vx = (nx > 0) ? 0.052f : -0.052f;
		break;
	}
	}

}

//CGhost* CGhost::__instance = NULL;
//CGhost* CGhost::GetInstance()
//{
//	if (__instance == NULL) __instance = new CGhost();
//		return __instance;
//}
