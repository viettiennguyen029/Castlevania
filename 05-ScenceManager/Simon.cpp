#include "Simon.h"

CSimon::CSimon() :CGameObject()
{

}



void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	// Calculate x,y
	CGameObject::Update(dt);
	// Simple fall down
	vy += SIMON_GRAVITY * dt;
	// CheckCollisionWithGround(dt, coObjects);

	vector <LPCOLLISIONEVENT> coEvents;
	vector <LPCOLLISIONEVENT> coEventsResult;

	// turn off collision when simon is die
	if (state != SIMON_STATE_DIE)
	{
		CalcPotentialCollisions(coObjects, coEvents);
	}

	// No collision, process normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		if (vy>0.03f)
		{
			isFalling = true;
		}
		
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		/*
		Collision logic with others objects
		*/
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT  e = coEventsResult[i];
			if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->ny != 0)
				{
					vy = 0;
					isOnGround = true;
					isFalling = false;
					
				}
				//else					
					//y += dy;
			}

			// switching scene logic		
			else if (dynamic_cast<CPortal*> (e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*> (e->obj);
				DebugOut(L"[INFO] Switching to scene %d", p->GetSceneId());
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}

	// Clear up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
	}

}


void CSimon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_WALKING:
	{
		if (nx > 0) vx = SIMON_WALKING_SPEED;
		else vx = -SIMON_WALKING_SPEED;
		break;
	}

	case SIMON_STATE_IDLE:
	{
		vx = 0;
		break;
	}

	case SIMON_STATE_DIE:
	{
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	}

	case SIMON_STATE_JUMP:
	{
		isOnGround = false;
		vy = -SIMON_JUMP_SPEED_Y;
		// animations[SIMON_ANI_JUMP]->SetAniStartTime(GetTickCount());	
	}

	case SIMON_STATE_SIT:
	{
		vx = 0;
		break;
	}
	}
}

void CSimon::Render()
{
	int ani = -1;

	if (isFalling == true)
	{
		state = SIMON_STATE_SIT;
	}

	if (state == SIMON_STATE_ATTACK) ani = SIMON_ANI_ATTACK;
	else if (state == SIMON_STATE_JUMP) ani = SIMON_ANI_JUMP;
	else if (state == SIMON_STATE_SIT) ani = SIMON_ANI_SIT;
	else
	{
		if (vx == 0) ani = SIMON_ANI_IDLE;
		else	ani = SIMON_ANI_WALKING;
	}

	int alpha = 255;
	animation_set->at(ani)->Render(x, y, nx, alpha);
	RenderBoundingBox();
}

void CSimon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + SIMON_BBOX_WIDTH;
	bottom = y + SIMON_BBOX_HEIGHT;
}

/*void CSimon::StartAttacking()
{
	if (startAttackingTime > 0) return;

	if (state != SIMON_STATE_JUMP) vx = 0;

	if (state == SIMON_STATE_SIT)
		SetState(SIMON_STATE_SIT_ATTACK);
	else
		SetState(SIMON_STATE_ATTACK);

	startAttackingTime = GetTickCount();
	//animations[SIMON_ANI_JUMP]->SetAniStartTime(GetTickCount());
}

void CSimon::StartJumping()
{
	SetState(SIMON_STATE_JUMP);
	isOnGround = false;
	startJumpingTime = GetTickCount();
	//animations[SIMON_ANI_JUMP]->SetAniStartTime(GetTickCount());
}*/

