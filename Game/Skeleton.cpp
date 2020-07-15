#include "Skeleton.h"
#include "Brick.h"

void CSkeleton::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + SKELETON_BBOX_WIDTH;
	bottom = top + SKELETON_BBOX_HEIGHT;
}

void CSkeleton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vy += 0.0018f * dt;

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
		x += dx;
		y += dy;
	}

	else
	{
		float min_tx, min_ty, nx, ny;
		float rdx, rdy;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx;
		y += min_ty * dy;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT  e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{				
				// The limmied of the knight is the width of the bricks under its feet
				CBrick* b = dynamic_cast<CBrick*>(e->obj);

				this->start_x = b->x + 1;
				float end_x = start_x + b->width - SKELETON_BBOX_WIDTH/2;

				if (e->ny != 0)
				{
					vy = 0;
					y += ny * 0.4f; 
				}

				if (x >= end_x || x <= start_x)
				{
					vx = -vx;
				}
			}
		}

		// clean up collision events
		for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}

}

void CSkeleton::Render()
{
	animation_set->at(state)->Render(x, y, nx);
}

CSkeleton::CSkeleton(float x, float y)
{
	this->start_x = x;
	this->start_y = y;
	this->healthPoint = 2;
	SetVisible(false);
	SetState(SKELETON_STATE_DANCING);
	this->vx = 0.03f;
}

void CSkeleton::SetState(int state)
{
	CGameObject::SetState(state);
}

CSkeleton* CSkeleton::__instance = NULL;
CSkeleton* CSkeleton::GetInstance()
{
	if(__instance == NULL) __instance = new CSkeleton();
	return __instance;
}
