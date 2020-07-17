#include "Bones.h"

CBones* CBones::__instance = NULL;
CBones* CBones::GetInstance()
{
	if (__instance == NULL) __instance = new CBones();
	return __instance;
}


CBones::CBones()
{
	SetVisible(false);
}

void CBones::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BONES_BBOX_WIDTH;
	bottom = y + BONES_BBOX_HEIGHT;
}

void CBones::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	vy += 0.0005f * dt; //gravity

	if (abs(x - start_x) <= 6) 
	{
		vx = (nx > 0) ? BONES_FLY_SPEED_VX * dt : -BONES_FLY_SPEED_VX * dt;
		//vy = -0.22f;

		int r = rand() % 2;
		if (r == 0)
		{
			vy = -0.13f;
		}
		else
		{
			vy = -0.22f;
		}
	}
	else
	{
		vx = (nx > 0) ? BONES_FLY_SPEED_VX * dt : -BONES_FLY_SPEED_VX * dt;
		if (abs(x - start_x) >= 70)
			vy = 0.3f;
	}

	if (!IsInViewport())
	{
		SetVisible(false);
	}
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
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx;
		y += min_ty * dy;

	}

	// Clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CBones::Render()
{
	animation_set->at(0)->Render(x, y, nx);
}