#include "Zombie.h"
#include "Brick.h"

CZombie::CZombie()
{
	this->healthPoint = 1;
	//this->vx = 0.06f;
	SetOrientation(-1);
	SetState(ZOMBIE_STATE_MOVING);
}

void CZombie::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + ZOMBIE_BBOX_WIDTH;
	bottom = top + ZOMBIE_BBOX_HEIGHT;
}

void CZombie::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving)
{
	if (stopMoving == true)
	{
		stop = true;
		return;
	}
	else
	{
		stop = false;
	}

	CGameObject::Update(dt);

	// Simple fall down
	vy += ZOMBIE_GRAVITY * dt;

	if (nx == 1) vx = ZOMBIE_MOVING_SPEED;
	else if (nx -1) vx = -ZOMBIE_MOVING_SPEED;

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

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT  e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->ny ==-1)
				{
					y = y  -0.2f;
					vy = 0;
				}
				
			}

		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CZombie::Render()
{
	if (stop)
	{
		int currentFrame = animation_set->at(0)->GetCurrentFrame();
		animation_set->at(0)->SetCurrentFrame(currentFrame);
		animation_set->at(0)->RenderByFrame(currentFrame, nx, x, y);
	}
	else
		animation_set->at(0)->Render(x, y, nx);
	//animation_set->at(ZOMBIE_STATE_MOVING)->Render(x, y, nx);
}

void CZombie::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ZOMBIE_STATE_MOVING:
		vx = ZOMBIE_MOVING_SPEED;
		vy = 0;
		break;
	}
}
