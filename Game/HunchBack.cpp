#include "HunchBack.h"
#include "Simon.h"


void CHunchBack::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + HUNCH_BACK_BBOX_WIDTH;
	bottom = y + HUNCH_BACK_BBOX_HEIGHT;
}

void CHunchBack::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	
	// Fall down
	vy += HUNCHBACK_GRAVITY * dt;	

	//Activating Hunch Back to jump down
	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);

	float xH, yH;
	this->GetPosition(xH, yH);

	if (xS - xH <= 98 && yS - yH <= 34) // Active Point 
	{
		SetState(HUNCH_BACK_STATE_JUMP);
	}	

	if (xH -xS>100)
	{
		SetOrientation(-1);
		//SetState(HUNCH_BACK_STATE_IDLE);
	}

	if (xS - xH > 100)
	{
		SetOrientation(1);
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

		x += min_tx * dx+ nx*0.4f;
		if (ny <= 0)
			y += min_ty * dy + ny * 0.4f;
		

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT  e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
			
				if (nx != 0) x += dx;
				if (e->ny != 0)
				{
					if (e->ny == -1) vy = 0; // hunch back standing on brick
					else 	y += dy; //hunch back can jump through brick
				}

				if (hopping)
				{
					// Random vy with small and large hops
					float minvy = -0.1f;
					float maxvy = -0.2f;
					vy = minvy + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxvy - minvy)));
				}
				
			
			}

			else
			{
				if (e->nx != 0)
					x += dx;
				else if (e->ny < 0) 
					y += dy;
			}
		}
	}


	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CHunchBack::Render()
{
	int ani = 0;
	switch (this->state)
	{
	case HUNCH_BACK_STATE_IDLE:
		ani = 0;
		
		break;
	case HUNCH_BACK_STATE_JUMP:
		ani = 1;
		break;
		
	default:
		break;
	};
	animation_set->at(ani)->Render(x, y,nx);	
}

CHunchBack::CHunchBack()
{
	this->hopping = false;
	this->healthPoint = 2;
	vx = vy = 0;
	SetState(HUNCH_BACK_STATE_IDLE);
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
		hopping = true;
		if (nx == 1) vx = HUNCH_BACK_JUMP_SPEED_X;
		else vx = -HUNCH_BACK_JUMP_SPEED_X;
		break;
	}
		
	}
	this->state = state;
}
