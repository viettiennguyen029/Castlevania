#include "Simon.h"
#include "Candle.h"

CSimon::CSimon(float x, float y) :CGameObject()
{
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	SetState(SIMON_STATE_IDLE);
	whip = new CWhip();
}

void CSimon::Update(DWORD dt, vector <LPGAMEOBJECT>* coObjects)
{
	// Calculate x,y
	CGameObject::Update(dt);
	whip->Update(dt, coObjects);

	// Simple fall down
	vy += SIMON_GRAVITY * dt;
	
	// Simple logic with screen edge
	if (vx < 0 && x < 0) x = 0;

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
				DebugOut(L"[INFO] Collision Simon and Candle %d %d\n", e->nx, e->ny);
				// Process normally
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}

			// Collision logic with Brick 
			else if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->ny !=0) vy = 0;				
			}

			// Collision logic with tems
			else if (dynamic_cast<ItemBigHeart*>(e->obj))
			{
				DebugOut(L"[ITEMS] Heart Collected \n");
				if (e->nx != 0 || e->ny != 0)
				{					
					e->obj->SetVisible(false);
				}				
			}
			else if (dynamic_cast<ItemChain*>(e->obj))
			{
				
				if (e->nx != 0 || e->ny != 0)
				{
					e->obj->SetVisible(false);
					this->whip->PowerUp();
					DebugOut(L"[INFO] WHIP UPGRADED \n");
				}
			}
			else if (dynamic_cast<ItemDagger*>(e->obj))
			{
				DebugOut(L"[ITEMS] Dagger Collected \n");
				if (e->nx != 0 || e->ny != 0)
				{
					e->obj->SetVisible(false);
					//subWeapon = true;
				}
			}

			// switching scene logic		
			else if (dynamic_cast<CPortal*> (e->obj))
			{
				DebugOut(L"[INFO] Portal detection ! \n");
				CPortal* p = dynamic_cast<CPortal*> (e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}

			else
			{
				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;				
			}
		}
	}

	// Clear up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
	}

	// Check collsion when simon attacking
	if (state == SIMON_STATE_ATTACK || state == SIMON_STATE_SIT_ATTACK)
	{
		whip->SetOrientation(nx);
		whip->SetWhipPosition(D3DXVECTOR2(x, y), isStanding);

		if (animation_set->at(state)->GetCurrentFrame() == 2) // Only check collsion at the last frame of the whip
		{
			for (UINT i = 0; i < coObjects->size(); i++)
			{
				LPGAMEOBJECT temp = coObjects->at(i);
				if (dynamic_cast<CCandle*>(temp))
				{
					CCandle* candle = dynamic_cast<CCandle*> (temp);
					float left, top, right, bottom;
					temp->GetBoundingBox(left, top, right, bottom);

					if (whip->isColliding(left, top, right, bottom) == true)
					{
						DebugOut(L"[INFO]Whip Collision with Torch \n");						
						temp->SetState(CANDLE_DESTROYED);				
						temp->animation_set->at(CANDLE_DESTROYED)->SetAniStartTime(GetTickCount());
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

	case SIMON_STATE_IDLE:
	{
		isStanding = true;
		vx = 0;
		break;
	}

	case SIMON_STATE_WALKING:
	{
		if (nx > 0) vx = SIMON_WALKING_SPEED;
		else vx = -SIMON_WALKING_SPEED;
		break;
	}	

	case SIMON_STATE_DIE:
	{
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	}

	case SIMON_STATE_JUMP:
	{
		isStanding = true;
		vy = -SIMON_JUMP_SPEED_Y;			
		break;
	}

	case SIMON_STATE_SIT:
	{
		isStanding = false;
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
	case SIMON_STATE_GO_UPSTAIR:
	{
		if (nx > 0)
		{
			vx = SIMON_GO_UPSTAIR_SPEED;
			vy = -SIMON_GO_UPSTAIR_SPEED;
		}
		// handle nx<0
	}
	}
}

void CSimon::Reset()
{
	SetState(SIMON_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void CSimon::Render()
{
	int ani = -1;

	if (state == SIMON_STATE_DIE)
	{
		ani = SIMON_ANI_IDLE;
	}
	else if (state == SIMON_STATE_ATTACK) ani = SIMON_ANI_ATTACK;
	else if (state == SIMON_STATE_SIT_ATTACK) ani = SIMON_ANI_SIT_ATTACK;
	else if (state == SIMON_STATE_JUMP) ani = SIMON_ANI_JUMP;
	else if (state == SIMON_STATE_SIT) ani = SIMON_ANI_SIT;
	else if (state == SIMON_STATE_GO_UPSTAIR) ani = SIMON_ANI_GO_UPSTAIR;
	else
	{
		if (vx == 0) ani = SIMON_ANI_IDLE;
		else	ani = SIMON_ANI_WALKING;
	}
	
	int alpha = 255;
	animation_set->at(ani)->Render(x, y, nx, alpha);
	// RenderBoundingBox();	

	// Whip rendering
	// Need to update
	if (state == SIMON_STATE_ATTACK || state == SIMON_STATE_SIT_ATTACK) 
	{
		// if (subWeapon==false)
		whip->Render(animation_set->at(ani)->GetCurrentFrame());
	}
}

void CSimon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + SIMON_BBOX_WIDTH;
	bottom = y+ SIMON_BBOX_HEIGHT;
}
