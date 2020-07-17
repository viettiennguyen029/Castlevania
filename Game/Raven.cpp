#include "Raven.h"
#include "Simon.h"

CRaven::CRaven()
{
	SetState(RAVEN_STATE_IDLE);
}

void CRaven::Render()
{
	int ani = -1;
	if (vx == 0)
		ani = 0;
	else ani = 1;
	animation_set->at(ani)->Render(x, y, nx);
}

void CRaven::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case RAVEN_STATE_IDLE:
	{
		vx = 0;
		vy = 0;
		break;
	}
	case RAVEN_STATE_FLYING:
	{
		/*vx = -0.02f;
		vy = 0.01f;*/
		break;
	}

	}
}

void CRaven::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);

	// Raven detecting Simon
	if (buffSpeed == false)
	{
		nx = (x > xS) ? -1 : 1;
		ny = (y > yS) ? -1 : 1;
	}

	// Caculate distance to active raven 
	/*
	NOTES: Recaculate this point
	*/
	if (x - xS < 80)
	{
		if (state == RAVEN_STATE_IDLE)
		{
			activeRaven = true;
			active_raven_start = GetTickCount();
			SetState(RAVEN_STATE_FLYING);

			//float VX = abs(target->GetPositionX() - this->x);
			// float VY = abs(target->GetPositionY() - this->y);
	
			vx = (float)(RAVEN_FLYING_DOWN_SPEED)*nx;
			vy = (float)(RAVEN_FLYING_DOWN_SPEED)*ny;
		}
	}

	// Stop a bit to find player
	if (GetTickCount() - active_raven_start > RAVEN_FLYING_DOWN_TIME == true && state != RAVEN_STATE_IDLE)
	{
		active_raven_start = 0;
		attackingPlayer = true;
		attackingPlayer_start = GetTickCount();
		vx = 0;
		vy = 0;
	}

	// Start attacking
	if (GetTickCount() - attackingPlayer_start > RAVEN_DELAY_TIME == true && attackingPlayer == true && buffSpeed == false)
	{
		attackingPlayer_start = 0;
		buffSpeed = true;
		float VX = abs(xS - this->x);
		float VY = abs(yS- this->y);
		vx = (float)(VX * 0.0015) * nx;
		vy = (float)(VY * 0.0015) * ny;

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

void CRaven::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + RAVEN_BBOX_WIDTH;
	bottom = y + RAVEN_BBOX_WIDTH;
}
