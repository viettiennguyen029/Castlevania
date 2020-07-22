#include "ItemHolyWater.h"
#include "Brick.h"

ItemHolyWater::ItemHolyWater()
{
	SetVisible(false);
}

void ItemHolyWater::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving )
{
	CGameObject::Update(dt);

	vy += ITEM_GRAVITY * dt;				// simple fall down

	if (visible)
	{
		if (start_visible < 5000)
		{
			start_visible += dt;
		}
		else
		{
			SetVisible(false);
			start_visible = 0;
		}
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

		for (UINT i = 0; i < coEventsResult.size(); ++i)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				// Block brick
				if (e->ny != 0)
				{
					y += 0.4f * e->ny;
					vy = 0;
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void ItemHolyWater::Render()
{
	animation_set->at(0)->Render(x, y, 1);
}

void ItemHolyWater::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ITEM_HOLY_WATER_BBOX_WIDTH;
	bottom = y + ITEM_HOLY_WATER_BBOX_HEIGHT;
}


