#include "HunchBack.h"
#include "Simon.h"


void CHunchBack::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + HUNCH_BACK_BBOX_WIDTH;
	bottom = y + HUNCH_BACK_BBOX_HEIGHT;
}

void CHunchBack::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving)
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

	//Activating Hunch Back to jump down
	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);
	
	if (xS -this->x<= DISTANCE_ACTIVE_HUNCH_BACK && startWaitingTime == 0)
	{
		startWaitingTime = GetTickCount();
	}


	if (startWaitingTime > 0)
	{
		if (GetTickCount() - startWaitingTime >= HUNCH_BACK_WATING_TIME)
		{
			SetState(HUNCH_BACK_STATE_JUMP);

			if (CSimon::GetInstance()->GetUntouchable() == 1)
			{
				if (grounding) LargeHopping();
				vy+= LARGE_HOPPING_GRAVITY * dt;
			}
			else
			{
				if (grounding)
				{
					Hopping();
					DetectPlayer();					
				}
				vy += HUNCH_BACK_GRAVITY * dt;
			}			
		}

		else
		{
			vy += HUNCH_BACK_GRAVITY * dt;
			vx = vy = 0;
			hopping = false;
		}
	}
	
	// untouchable
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

			if (dynamic_cast<CBrick*>(e->obj) || 
				dynamic_cast<CBreakWall*>(e->obj))
			{
			
				if (e->ny < 0)
				{
					if (hopping)
					{
						hopping = false;
						startWaitingTime = 0;
						//DebugOut(L"Hunchback Grounding\n");
					}
					grounding = true;
					y += ny * 0.4f;
				}

				if (e->nx != 0)
				{
					LargeHopping();
					DetectPlayer();
				}
			}
		}
	}
	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];

	if (this->IsInViewport() == false && startWaitingTime != 0)
		SetVisible(false);
}

void CHunchBack::Hopping()
{
	if (!hopping)
	{
		vy = -SMALL_HOPPING_VY;
		hopping = true;
		dy = this->dt * vy;
		vx = SMALL_HOPPING_VX * nx;
		grounding = false;
	}
}

void CHunchBack::LargeHopping()
{
	if (!hopping)
	{
		vy = -LARGE_HOPPING_VY;
		hopping = true;
		vx = LARGE_HOPPING_VX * nx;
		grounding = false;
	}
}

void CHunchBack::Render()
{
	int ani = 0;
	switch (this->state)
	{
	case HUNCH_BACK_STATE_IDLE:
		ani = 0; // 2 frames		
		break;
	case HUNCH_BACK_STATE_JUMP:
		if (hopping) ani = 1; //frame 1 Jumping
		else ani = 2; //  frame  2 Grounding ?????
		break;
		
	default: 
		break;
	};


	if (stop)
	{
		int currentFrame = animation_set->at(ani)->GetCurrentFrame();
		animation_set->at(ani)->SetCurrentFrame(currentFrame);
		animation_set->at(ani)->RenderByFrame(currentFrame, nx, x, y);
	}
	else
		animation_set->at(ani)->Render(x, y, nx);
}

CHunchBack::CHunchBack()
{
	this->hopping = false;
	this->healthPoint = 2;
	vx = vy = 0;
	SetState(HUNCH_BACK_STATE_IDLE);
}

void CHunchBack::DetectPlayer()
{
	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);

	if (xS > x + HUNCH_BACK_BBOX_WIDTH)
	{
		SetOrientation(1);
		//DebugOut(L"Hunchback turnover to hopping to Simon\n");
	}
	else if (xS + SIMON_BBOX_WIDTH < x)
	{
		SetOrientation(-1);
		//DebugOut(L"Hunchback turnover to hopping to Simon\n");
	}
}

void CHunchBack::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case HUNCH_BACK_STATE_IDLE:
	{
		vx = 0;
		vy = 0;
		break;
	}

	case HUNCH_BACK_STATE_JUMP:
	{
		break;
	}	
	}
}
