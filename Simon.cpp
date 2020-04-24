#include "Simon.h"
#include "Candle.h"

CSimon::CSimon() :CGameObject()
{
	//	SetState(SIMON_STATE_IDLE);
	whip = new CWhip();
}


void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	// Calculate x,y
	CGameObject::Update(dt);
	// Simple fall down
	vy += SIMON_GRAVITY * dt;
	// CheckCollisionWithGround(dt, coObjects);

	if (vx < 0 && x < 0) x = 0;

	vector <LPCOLLISIONEVENT> coEvents;
	vector <LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
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

			// collision logic with Candle
			if (dynamic_cast<CCandle*>(e->obj))
			{
				DebugOut(L"Collision Simon and Candle %d %d\n", e->nx, e->ny);
				// Process normally
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;

			}

			else if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->ny != 0)
				{
					vy = 0;
					isOnGround = true;
					isFalling = false;
				}

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

	// Check collsion when simon attacking
	if (state == SIMON_STATE_ATTACK|| state == SIMON_STATE_SIT_ATTACK)
	{		
		whip->SetOrientation(nx);
		whip->SetWhipPosition(x, y);

		if (animation_set->at(4)->GetCurrentFrame() == 2) // Only check collsion when simon 
		{
			if (nx > 0)
			{
				whip->SetPosition(x + 30, y + 4);

			}
			else
				whip->SetPosition(x - 20,y + 4);

			for (UINT i = 0; i < coObjects->size(); i++)
			{
				LPGAMEOBJECT temp = coObjects->at(i);
			
				if (dynamic_cast<CCandle*>(temp))
				{
					CCandle* e = dynamic_cast<CCandle*> (temp);
					float left, top, right, bottom;
					e->GetBoundingBox(left, top, right, bottom);
					if (whip->isColliding(left, top, right, bottom) == true)
					{
						DebugOut(L"collision\n");
					}
				}
			}
		}
		
		
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
		animation_set->at(SIMON_ANI_JUMP)->SetAniStartTime(GetTickCount());
		break;
	}

	case SIMON_STATE_SIT:
	{
		vx = 0;
		vy = 0;
		break;
	}
	case SIMON_STATE_ATTACK:
	{
		animation_set->at(SIMON_ANI_ATTACK)->Reset();
		animation_set->at(SIMON_ANI_ATTACK)->SetAniStartTime(GetTickCount());
		break;
	}

	case SIMON_STATE_SIT_ATTACK:
	{
		animation_set->at(SIMON_ANI_SIT_ATTACK)->Reset();
		animation_set->at(SIMON_ANI_SIT_ATTACK)->SetAniStartTime(GetTickCount());
		break;
	}
	}
}

void CSimon::Render()
{
	int ani = -1;

	// simon luôn co chân khi rơi xuóng
	if (isFalling == true && isAttacking() == false)
	{
		state = SIMON_STATE_SIT;
	}

	if (state == SIMON_STATE_DIE)
	{
		ani = SIMON_ANI_IDLE;
	}
	else if (state == SIMON_STATE_ATTACK) ani = SIMON_ANI_ATTACK;
	else if (state == SIMON_STATE_SIT_ATTACK) ani = SIMON_ANI_SIT_ATTACK;
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

	// Rendering whip
	if (isAttacking() == true)
	{
		int currentFrame = animation_set->at(SIMON_ANI_ATTACK)->GetCurrentFrame();
		whip->Render(currentFrame);
		// currentFrame = -1;

	}

}

void CSimon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	//if (nx > 0)
	//{
	left = x - SIMON_BBOX_WIDTH ;
	top = y;
	right = x + SIMON_BBOX_WIDTH ;
	bottom = y + SIMON_BBOX_HEIGHT;
	//	}
		/*else
		{
			left = x+SIMON_BBOX_WIDTH;
			top = y;
			right = x + SIMON_BBOX_WIDTH;
			bottom = y + SIMON_BBOX_HEIGHT;
		}*/

}

void CSimon::Simon_Jumping()
{
	if (isOnGround == false || isAttacking() == true) return;
	SetState(SIMON_STATE_JUMP);
}

void CSimon::Simon_Attacking()
{
	if (isAttacking() == true) return;
	if (isFalling == true) return;

	// Đứng đánh, nhảy đánh
	if (state == SIMON_STATE_IDLE || state == SIMON_STATE_JUMP)
	{
		SetState(SIMON_STATE_ATTACK);
	}
	// Ngồi đánh
	else if (state == SIMON_STATE_SIT)
	{
		SetState(SIMON_STATE_SIT_ATTACK);
	}

}

bool CSimon::isAttacking()
{
	return state == SIMON_STATE_ATTACK || state == SIMON_STATE_SIT_ATTACK;
}






