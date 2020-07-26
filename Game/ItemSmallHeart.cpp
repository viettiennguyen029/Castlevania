#include "ItemSmallHeart.h"
#include "Brick.h"
#include "StairTop.h"
#include "Simon.h"

ItemSmallHeart::ItemSmallHeart()
{	
	this->visible = false;	
	this->vx_variability = 0.0004f;
}

void ItemSmallHeart::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving)
{	
	CGameObject::Update(dt);

	//vy += ITEM_GRAVITY * dt;				// simple fall down	

	vy = 0.04;

	// Horizontal movement
	vx += vx_variability * dt;
	if (abs(vx) > 0.1f && vx * vx_variability > 0) vx_variability = -vx_variability;

	//if (vy != 0)
	//{
	//	vx += vx_variability * dt;
	//	vy = 0.03f;

	//	if (vx >= 0.1f || vx <= -0.1f)
	//		vx_variability *= -1;
	//}
	//else vy = 0;

	if (visible)
	{	
		if (start_visible < ITEM_LIFESPAN)
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

		for (UINT i = 0; i < coEventsResult.size(); ++i)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				// Block brick,stop moving in horizontal 
				if (e->ny <0)
				{
					//y += 0.4f * e->ny;
					vy = 0;
					y += 0.2f * e->ny;
					//vx_variability = 0;
				}				
				vy = vx = 0;
			}

			else if (dynamic_cast<CStairTop*>(e->obj) ||  (dynamic_cast<CStairBottom*>(e->obj)))
			{
				// Process normally
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
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
