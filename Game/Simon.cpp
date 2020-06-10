#include "Simon.h"
#include "Candle.h"

CSimon* CSimon::__instance = NULL;
CSimon* CSimon::GetInstance()
{
	if (__instance == NULL)
		__instance = new CSimon();
	return __instance;
}

void CSimon::GoUpStair()
{
	// Check if Simon is on stairs and want to go up
	if (onStairs ==0)
	{
		LPGAMEOBJECT stairs = NULL;

		// Check if Simon is on a stairs-start
		for (UINT i = 0; i < ovObjects.size(); ++i)
		{
			if (dynamic_cast<CStairBottom*>(ovObjects[i]) || dynamic_cast<CVariousStair*>(ovObjects[i]))
			{				
				stairs = ovObjects[i];
				if (dynamic_cast<CVariousStair*>(ovObjects[i]))
				{
					stairs->SetOrientation(-nx);
				}
				break;
			}		
		}

		// Do nothing if there is no stairs overlapped
		if (stairs == NULL)
		{
			state = SIMON_STATE_IDLE;
			return;
		}

		// Enable go up stair
		float xS, yS;
		stairs->GetPosition(xS, yS);
		nx = stairs->GetOrientation();
		this->vx = nx * SIMON_GO_UPSTAIR_SPEED;
		this->vy = -SIMON_GO_UPSTAIR_SPEED;
		onStairs = 1;		
		
	}
	else 
	{
		if (onStairs == -1) // Simon is going down stair and want to go back
		{
			this->nx = -nx;
			onStairs = 1;
		}
		this->vx = nx * SIMON_GO_UPSTAIR_SPEED;
		this->vy = -SIMON_GO_UPSTAIR_SPEED;
	}
}

void CSimon::ProceedOnStairs()
{
	// Try getting out the stairs

	LPGAMEOBJECT stairs = NULL;
	if (onStairs ==1 )
	{
		// Condition to check if simon is Overlapping with various stair or not?? 

		for (UINT i = 0; i < ovObjects.size(); ++i)
		{
			LPGAMEOBJECT temp = ovObjects[i];
		
			if (dynamic_cast<CStairTop*>(ovObjects[i]) || ((dynamic_cast<CVariousStair*>(ovObjects[i]))&& this->nx ==-1))
				{					
					stairs = ovObjects[i];
					break;
				}
						
		}
		if (stairs == NULL) return;

		float xS, yS;
		this->nx = stairs->GetOrientation();
		stairs->GetPosition(xS, yS);

		// Check if simon has reached the stairs exit
		if (y < yS && stairs->GetOrientation()==nx)
		{
			y = yS - 0.2f;
			onStairs = 0;
			vx = vy = 0;
		}

	}
	else if (onStairs == -1)
	{
		for (UINT i = 0; i < ovObjects.size(); ++i)
			if (dynamic_cast<CStairBottom*>(ovObjects[i]) || 
				( dynamic_cast<CVariousStair*>(ovObjects[i])&& this ->nx ==-1))
			{
				stairs = ovObjects[i];
				break;
			}

		if (stairs == NULL) return;

		float xS, yS;
		stairs->GetPosition(xS, yS);

		// Check if simon has reached the stairs exit
		if (y > yS)
		{
			y = yS - 0.2f;
			onStairs = 0;
			vx = vy = 0;
		}

	}

}

void CSimon::GoDownStair()
{
	// Check if Simon is on stairs and want to go down
	if (onStairs == 0)
	{
		LPGAMEOBJECT stairs = NULL;

		// Check if Simon is on a stairs-start 
		for (UINT i = 0; i < ovObjects.size(); ++i)
		{
			if (dynamic_cast<CStairTop*>(ovObjects[i]) || 
				dynamic_cast<CVariousStair*>(ovObjects[i]))
			{				
				stairs = ovObjects[i];
				if (dynamic_cast<CVariousStair*>(ovObjects[i])) stairs->nx = -nx;				
				break;
			}
		}

		// Do nothing if there is no stairs overlapped
		if (stairs == NULL)
		{
			state = SIMON_STATE_IDLE;
			return;
		}

		// Enable go down stair
		float xS, yS;
		stairs->GetPosition(xS, yS);
		nx =- stairs->GetOrientation();
		this->vx = nx * SIMON_GO_UPSTAIR_SPEED;
		this->vy = SIMON_GO_UPSTAIR_SPEED;
		onStairs = -1;

	}
	else
	{
		if (onStairs == 1) // Simon is going up stair and want to go back
		{
			this->nx = -nx;
			onStairs = -1;
		}
		this->vx = nx * SIMON_GO_UPSTAIR_SPEED;
		this->vy = SIMON_GO_UPSTAIR_SPEED;
	}
}


//void CSimon::StartAutoMove(float vx, float xDestination)
//{
//	if (!autoMove)
//	{
//		autoMoveInfo.xDes = xDestination;
//		autoMoveInfo.vx = vx;
//
//		autoMove = true;
//	}
//	// Proceed Auto Move
//
//}


CSimon::CSimon(float x, float y) :CGameObject()
{
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	// this->autoMove = false;
	SetState(SIMON_STATE_IDLE);
    whip = new CWhip();
	dagger = new CDagger();
}

void CSimon::Update(DWORD dt, vector <LPGAMEOBJECT>* coObjects)
{
	// Calculate x,y
	CGameObject::Update(dt);
	whip->Update(dt, coObjects);
	dagger->Update(dt, coObjects);

	// Simple fall down
	vy += SIMON_GRAVITY * dt;
	
	// Simple logic with screen edge
	if (vx < 0 && x < 0) x = 0;

	// Discoloration time logic
	if (powerUp)
	{
		if (discolorationTime < SIMON_DISCOLOR_TIME)
		{
			vx = 0;
			discolorationTime += dt;
		}			
		else
		{
			powerUp = false;
			discolorationTime = 0;
		}
	}

	// Overlapping
	ovObjects.clear();
	for (UINT i = 0; i < coObjects->size(); ++i)
	{
		if (this->IsOverlapping(coObjects->at(i)))
			ovObjects.push_back(coObjects->at(i));
	}


	// Being On Stairs
	if (onStairs != 0)
	{
		ProceedOnStairs();
		vy = 0;
	}

	vector <LPCOLLISIONEVENT> coEvents;
	vector <LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
		
	// turn off collision when simon is die
	if (state != SIMON_STATE_DIE )
	{
		
		CalcPotentialCollisions(coObjects, coEvents);
	}

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SIMON_DEFLECT_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision, process normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;			
		//DebugOut(L"No Collision!\n");
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
				// DebugOut(L"[INFO] Collision Simon and Candle %d %d\n", e->nx, e->ny);
				// Process normally
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}

			// Collision logic with Brick 
			else if (dynamic_cast<CBrick*>(e->obj))
			{
				onMovingPlatform = false;
				if (onStairs == 0)
				{
					if (e->ny != 0)
					{
						//vy = 0;
						if (e->ny == -1) vy = 0; // simon standing on brick
						else 	y += dy; //simon can jump through brick
					}
				}
				else  //Turn off collision with brick when simon is on stairs
				{							
					x += dx;
					y += dy;
				}
					
			}

			// Collision logic when Simon is on theMoving Platform
			else if (dynamic_cast<CMovingPlatform*>(e->obj))
			{			
				if (e->nx != 0) x += dx;

				CMovingPlatform* m = dynamic_cast<CMovingPlatform*> (e->obj);
				onMovingPlatform = true;
				this->vx = m->vx;
				vy = 0;
				DebugOut(L"On Moving Platform\n");
			}

			// Collision logic with tems
#pragma region Collision logic with items
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
					this->powerUp = true;
					this->SetState(SIMON_STATE_IDLE);
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
					//this->powerUp = true;
					//this->SetState(SIMON_STATE_IDLE);
					e->obj->SetVisible(false);
					subWeapon = true;
				}
			}

			else if (dynamic_cast<ItemBoomerang*>(e->obj))
			{
				DebugOut(L"[ITEMS] Boomerang Collected \n");
				if (e->nx != 0 || e->ny != 0)
				{
					e->obj->SetVisible(false);
					subWeapon = true;
				}
			}

			else if (dynamic_cast<ItemMoneyBag*>(e->obj))
			{
				DebugOut(L"[ITEMS] Money Bag Collected \n");
				if (e->nx != 0 || e->ny != 0)
				{
					e->obj->SetVisible(false);
				}
			}
#pragma endregion
			
			else if (dynamic_cast<CStairBottom*> (e->obj) || 
				dynamic_cast<CStairTop*> (e->obj) || 
				dynamic_cast<CVariousStair*>(e->obj))				
			{
				DebugOut(L"[INFO] Stair detection ! Direction: %d \n",e->nx);
				// Process normally
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
	
			}

			// switching scene logic		
			else if (dynamic_cast<CPortal*> (e->obj))
			{
			DebugOut(L"[INFO] Portal detection ! %d \n");
			CPortal* p = dynamic_cast<CPortal*> (e->obj);
			CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}

			else if (dynamic_cast<CBat*>(e->obj) ||	dynamic_cast<CBlack_Knight*>(e->obj))
			{
			if (e->nx != 0 && untouchable==0)
			{
			
				if (onStairs == 0)
				{
					StartUntouchable();
					DebugOut(L"[INFO] Enemies collision, Simon is Damaged \n");

					this->nx = (e->nx != 0) ?
						-(e->nx) :
						-(e->obj->GetOrientation());

					SetState(SIMON_STATE_DEFLECT);
				}
				else
				{
					StartUntouchable();
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
				}
			 }
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

		if (animation_set->at(state)->GetCurrentFrame() == 2 || 
			animation_set->at(SIMON_ANI_ATTACK_UPSTAIR)->GetCurrentFrame()==2||
			animation_set->at(SIMON_ANI_ATTACK_DOWNSTAIR)->GetCurrentFrame() == 2 ) // Only check collsion at the last frame of the whip
		{
			for (UINT i = 0; i < coObjects->size(); i++)
			{
				LPGAMEOBJECT temp = coObjects->at(i);
				if (dynamic_cast<CCandle*>(temp))
				{
					CCandle* candle = dynamic_cast<CCandle*> (temp);
					if (whip->IsOverlapping(temp))
					{
						DebugOut(L"[INFO]Whip Collision with Torch \n");						
						temp->SetState(CANDLE_DESTROYED);				
						temp->animation_set->at(CANDLE_DESTROYED)->SetAniStartTime(GetTickCount());
					}
				}
				else if (dynamic_cast<CBreakWall*>(temp))
				{
					CBreakWall* breakwall = dynamic_cast<CBreakWall*>(temp);
					if (whip->IsOverlapping(temp))
					{
						DebugOut(L"[INFO] Whip Collision with BreakWall \n");
						breakwall->Destroy();						
					}
				}

				else if (dynamic_cast<CBlack_Knight*>(temp))
				{
					//CBreakWall* breakwall = dynamic_cast<CBreakWall*>(temp);
					if (whip->IsOverlapping(temp))
					{
						DebugOut(L"[INFO] Whip Collision with Knight \n");
						temp->TakeDamage(1);
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
		if (onMovingPlatform == true) { ; }
		else vx = 0;
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
		if (onMovingPlatform==true) vx = 0;
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
		animation_set->at(SIMON_ANI_ATTACK_UPSTAIR)->Reset();
		animation_set->at(SIMON_ANI_ATTACK_DOWNSTAIR)->Reset();

		if (onStairs == 0) 
			animation_set->at(SIMON_ANI_ATTACK)->SetAniStartTime(GetTickCount());
		
		else
		{
			if (onStairs ==1) 
				animation_set->at(SIMON_ANI_ATTACK_UPSTAIR)->SetAniStartTime(GetTickCount());
			
			else 
				animation_set->at(SIMON_ANI_ATTACK_DOWNSTAIR)->SetAniStartTime(GetTickCount());			
		}
		break;
	}

	case SIMON_STATE_SIT_ATTACK:
	{
		animation_set->at(SIMON_ANI_SIT_ATTACK)->Reset();
		animation_set->at(SIMON_ANI_SIT_ATTACK)->SetAniStartTime(GetTickCount());
		break;
	}

	case SIMON_STATE_THROW:
	{
		vx = 0;
		animation_set->at(SIMON_ANI_THROW)->Reset();
		animation_set->at(SIMON_ANI_THROW)->SetAniStartTime(GetTickCount());
		break;
	}

	case SIMON_STATE_GO_UPSTAIR:
	{
		GoUpStair();
		break;
	}

	case SIMON_STATE_GO_DOWNSTAIR:
	{
		GoDownStair();
		break; 
	}

	case SIMON_STATE_DEFLECT:
	{
		vx = vy = dx = dy = 0;

		this->vx = (-this->nx) * SIMON_DEFLECT_SPEED_X;
		vy = -SIMON_DEFLECT_SPEED_Y;

		animation_set->at(SIMON_ANI_DEFLECT)->Reset();
		animation_set->at(SIMON_ANI_DEFLECT)->SetAniStartTime(GetTickCount());
		
		break;		
	}
	
	}
}

void CSimon::Reset()
{
	onStairs = 0;
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
	else if (state == SIMON_STATE_ATTACK || state == SIMON_STATE_THROW)
	{
		if (onStairs == 1)
		{
			animation_set->at(SIMON_ANI_ATTACK)->Reset();
			ani = SIMON_ANI_ATTACK_UPSTAIR;
		}
		else if (onStairs == -1)
		{
			ani = SIMON_ANI_ATTACK_DOWNSTAIR;
		}
		else ani = SIMON_ANI_ATTACK;

	}
	else if (state == SIMON_STATE_SIT_ATTACK) ani = SIMON_ANI_SIT_ATTACK;
	else if (state == SIMON_STATE_THROW) ani = SIMON_ANI_THROW;
	else if (state == SIMON_STATE_JUMP) ani = SIMON_ANI_JUMP;
	else if (state == SIMON_STATE_SIT) ani = SIMON_ANI_SIT;
	else if (state == SIMON_STATE_DEFLECT) ani = SIMON_ANI_DEFLECT;
	else if (onStairs !=0)
	{
		if (vx != 0)
		{
			if (onStairs == 1) ani = SIMON_ANI_GO_UPSTAIR;
			else if (onStairs == -1) ani = SIMON_ANI_GO_DOWNSTAIR;
		}
		else
		{
			if (onStairs == 1) ani = SIMON_ANI_IDLE_UPSTAIR;
			else if (onStairs == -1) ani = SIMON_ANI_IDLE_DOWNSTAIR;
		}
			
	}
	
	else
	{
		if (vx == 0)
		{
			if (powerUp == true) ani = SIMON_ANI_POWER_UP;
			else ani = SIMON_ANI_IDLE;
		}	
		else 
		{
			if (onMovingPlatform == true ) ani = SIMON_ANI_IDLE;
			else ani = SIMON_ANI_WALKING;
		}
	}
	
	int alpha = 255;
	
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, nx, alpha);
	// RenderBoundingBox();	

	// Whip rendering
	if (state == SIMON_STATE_ATTACK || state == SIMON_STATE_SIT_ATTACK) 
	{
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
