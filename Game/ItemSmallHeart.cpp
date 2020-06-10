#include "ItemSmallHeart.h"
#include "Brick.h"
ItemSmallHeart::ItemSmallHeart()
{	
	this->visible = false;	
	this->vx_variability = 0.0007f;
}

void ItemSmallHeart::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{	
	vy += ITEM_GRAVITY * dt;				// simple fall down	

	if (vy != 0)
	{
		vx += vx_variability * dt;		
		vy = 0.06f;

		if (vx >= 0.15f || vx <= -0.15f)
			vx_variability *= -1;
	}
	else vy = 0;	

	CGameObject::Update(dt);	
	
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

		for (UINT i = 0; i < coEventsResult.size(); ++i)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				// Block brick
				if (e->ny != 0 || e->nx!=0)
				{
					//y += 0.4f * e->ny;
					vy = 0;
				}
				vx = vx_variability = 0;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}


void ItemSmallHeart::Render()
{
	animation_set->at(0)->Render(x, y, 1);
}


void ItemSmallHeart::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ITEM_SMALL_HEART_BBOX_WIDTH;
	bottom = y + ITEM_SMALL_HEART_BBOX_HEIGHT;
}
