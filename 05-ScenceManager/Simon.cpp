#include "Simon.h"

CSimon::CSimon():CGameObject()
{

}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate x,y
	CGameObject::Update(dt);

	// Simple fall down
	vy += SIMON_GRAVITY * dt;

	vector <LPCOLLISIONEVENT> coEvents;
	vector <LPCOLLISIONEVENT> coEventsResult;

	// turn off collision when simon is die
	if (state != SIMON_STATE_DIE)
	{
		CalcPotentialCollisions(coObjects, coEvents);
	}

	// No collision, process normally
	if (coEvents.size()==0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx*0.4f;
		y += min_ty * dy + ny*0.4f;
		
		/*
		Collision logic with others objects
		*/
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT  e = coEventsResult[i];

			// switching scene logic
			if (dynamic_cast<CPortal*> (e->obj) ) 
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
	case SIMON_STATE_WALKING_RIGHT:
	{
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	}

	case SIMON_STATE_WALKING_LEFT:
	{
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
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
		vy = -SIMON_JUMP_SPEED_Y;
		break;
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
	
	if ( state == SIMON_STATE_DIE)
	{
		ani = SIMON_ANI_DIE;
	}

	else if (state == SIMON_STATE_JUMP )
	{
		if (nx > 0)
		{
			ani = SIMON_ANI_JUMP_RIGHT;
		}
		else ani = SIMON_ANI_JUMP_LEFT;
	}

	else if (state == SIMON_STATE_SIT)
	{
		if (nx > 0)
		{
			ani = SIMON_ANI_SIT_RIGHT;
		}
		else ani = SIMON_ANI_SIT_LEFT;
	}

	else if (state == SIMON_STATE_ATTACK)
	{
		if (nx > 0)
		{
			ani = SIMON_ANI_ATTACK_TYPE_1_RIGHT;
		}
		else ani = SIMON_ANI_ATTACK_TYPE_1_LEFT;
	}

	else
	{
		if (vx == 0)
		{
			if (nx > 0)
			{
				ani = SIMON_ANI_IDLE_RIGHT;
			}
			else ani = SIMON_ANI_IDLE_LEFT;
		}
		else if (vx > 0)
		{
			ani = SIMON_ANI_WALKING_RIGHT;
		}
		else ani = SIMON_ANI_WALKING_LEFT;	
	}

	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
	RenderBoundingBox();
}

void CSimon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + SIMON_BBOX_WIDTH;
	bottom = y + SIMON_BBOX_HEIGHT;
}
