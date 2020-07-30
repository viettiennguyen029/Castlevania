#include "Raven.h"
#include "Simon.h"

CRaven::CRaven()
{
	SetState(RAVEN_STATE_IDLE);
	this->damage = 2;
}

void CRaven::Render()
{
	//animation_set->at(state)->Render(x, y, nx);
	if (stop)
	{
		int currentFrame = animation_set->at(state)->GetCurrentFrame();
		animation_set->at(state)->SetCurrentFrame(currentFrame);
		animation_set->at(state)->RenderByFrame(currentFrame, nx, x, y);
	}
	else
		animation_set->at(state)->Render(x, y, nx);
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
		break;
	}

	}
}

float CRaven::CalcDistance(float x1, float y1, float x2, float y2)
{
	return (float)sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void CRaven::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving )
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
	x += dx;
	y += dy;

	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS,yS);

	if (buffSpeed == false) // Redirect player 
	{		
		nx = (x > xS) ? -1 : 1;
		ny = (y > yS) ? -1 : 1;
	}

	if (CalcDistance(x, y, xS, yS) <= DISTANCE_TO_ACTIVE_RAVEN && this->IsInViewport())
	{
		if (state == RAVEN_STATE_IDLE)
		{
			raven_change_state = GetTickCount();
			SetState(RAVEN_STATE_FLYING);
			float VX = abs(xS - this->x);
			float VY = abs(yS - this->y);

			vx = (float)(RAVEN_FLYING_DOWN_SPEED)*nx;
			vy = (float)(RAVEN_FLYING_DOWN_SPEED)*ny;
		}
	}

	if (GetTickCount()- raven_change_state > RAVEN_DELAY_TIME &&
		detectPlayer == false && reDetectPlayer == false && state != RAVEN_STATE_IDLE) //Stop a  bit 
	{
		detectPlayer = true;
		raven_find_target = GetTickCount();
		vx = 0;
		vy = 0;
	}

	if (GetTickCount() -raven_find_target > RAVEN_DETECT_TIME && detectPlayer == true && buffSpeed == false)
	{
		raven_find_target = 0;
		buffSpeed = true;
		raven_want_to_play_again = GetTickCount(); 
		doyouwanttobuildasnowman = true;
		float VX = abs(xS - this->x);
		float VY = abs(yS - this->y);
		vx = (float)(VX * RAVEN_ATTACK_VELOCITY) * nx;
		vy = (float)(VY * RAVEN_ATTACK_VELOCITY) * ny;
	}

	// Do you want to build a snow man
	if (GetTickCount() - raven_want_to_play_again > RAVEN_ATTACK_TIME && doyouwanttobuildasnowman == true)
	{
		buffSpeed = false;
		detectPlayer = false;
		doyouwanttobuildasnowman = false;
		reDetectPlayer = true;
		raven_find_target = GetTickCount();
		vx = vy = 0;
	}

	if (GetTickCount()- raven_find_target> RAVEN_DETECT_TIME
		&& reDetectPlayer == true && buffSpeed == false)
	{
		buffSpeed = true;
		float VX = abs(xS - this->x);
		float VY = abs(yS - this->y);
		vx = (float)(VX * RAVEN_ATTACK_VELOCITY) * nx;
		vy = (float)(VY * RAVEN_ATTACK_VELOCITY) * ny;
	}

	
}

void CRaven::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + RAVEN_BBOX_WIDTH;
	bottom = y + RAVEN_BBOX_WIDTH;
}
