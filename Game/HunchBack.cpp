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
	//Activating Hunch Back to jump down
	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);

	float xH, yH;
	this->GetPosition(xH, yH);

	if (xS-xH>90 && xS - xH <= 98 && yS - yH <= 34) // Active Point 
	{
		vx = 0.12f;
		vy -= 0.12f;
	}

	if (y < 145)
	{
		DebugOut(L"[Hunch back] Jump down \n");
		vx = 0.14f;
		vy += 0.016f;
	}
		

	CGameObject::Update(dt);

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
				if (e->ny != 0)
				{
					vx=vy = 0;
					y += ny * 0.4f;
				}
			}

			
		}
	}


	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CHunchBack::Render()
{
	int ani = -1;
	if (vx == 0)
		ani = 0;
	else
		ani = 1;
	animation_set->at(ani)->Render(x, y, nx);
}

CHunchBack::CHunchBack()
{
	this->healthPoint = 2;
	vx = vy = 0;
	SetState(HUNCH_BACK_STATE_IDLE);
}

void CHunchBack::SetState(int state)
{
	CGameObject::SetState(state);
}
