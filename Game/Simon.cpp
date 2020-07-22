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
				nx = stairs->GetOrientation();
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

		// Try to reach the right point of stairs
		float stair_x, stair_y;
		stairs->GetPosition(stair_x, stair_y);

		// Enable to go up stair
		if (x != stair_x-8)
		{
			StartAutoWalk(SIMON_WALKING_SPEED, stair_x-8);
		}
		else
		{
			//nx = stairs->GetOrientation();
			StartAutoWalk(nx * SIMON_GO_UPSTAIR_SPEED, -SIMON_GO_UPSTAIR_SPEED, SIMON_AUTO_UPSTAIR_TIME);
			onStairs = 1;
		}		
	}
	else 
	{
		if (onStairs == -1) // Simon is going down stair and want to go back
		{
			this->nx = -nx;
			onStairs = 1;
		}
		StartAutoWalk(nx * SIMON_GO_UPSTAIR_SPEED, -SIMON_GO_UPSTAIR_SPEED, SIMON_AUTO_UPSTAIR_TIME);
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
		float stair_x, stair_y;
		stairs->GetPosition(stair_x, stair_y);
		
		// Enable to go down stair
		if (x != stair_x-8)
		{
			StartAutoWalk(SIMON_WALKING_SPEED, stair_x-8);
		}
		else
		{
			nx = -stairs->GetOrientation();
			StartAutoWalk(nx * SIMON_GO_UPSTAIR_SPEED, SIMON_GO_UPSTAIR_SPEED, SIMON_AUTO_DOWNSTAIR_TIME);
			onStairs = -1;
		}

	}
	else
	{
		if (onStairs == 1) // Simon is going up stair and want to go back
		{
			this->nx = -nx;
			onStairs = -1;
		}
		StartAutoWalk(nx * SIMON_GO_UPSTAIR_SPEED, SIMON_GO_UPSTAIR_SPEED, SIMON_AUTO_DOWNSTAIR_TIME);
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

void CSimon::StartAutoWalk(float vx, float end_x)
{
	if (!autoWalk)
	{
		autoInfo.xDes = end_x;
		autoInfo.vx = vx;

		autoWalk = true;
	}
}

void CSimon::StartAutoWalk(float vx, float vy, DWORD time)
{
	// Can only perform one auto-walk at a time
	// NOTE: this autoMove flag will be turned off in Update() -> ProceedAutoMove()
	if (!autoWalk)
	{
		autoInfo.vx = vx;
		autoInfo.vy = vy;
		autoInfo.timeMove = time;

		autoWalk = true;
		auto_walk_start = GetTickCount();
	}
}

void CSimon::ProceedAutoWalk()
{
	if (auto_walk_start == 0)
	{
		if (autoInfo.vx != 0 &&
			autoInfo.xDes != x)
		{
			ReDirect(autoInfo.xDes);
			vx = nx * autoInfo.vx;
			dx = vx * dt;

			// Calibrate the dx if Simon has passed and is moving far away from the destination
			if (dx > 0)
			{
				if (x + dx > autoInfo.xDes)
					dx = autoInfo.xDes - x;
			}
			else
			{
				if (x + dx < autoInfo.xDes)
					dx = autoInfo.xDes - x;
			}
		}
		else
		{
			autoWalk = false;
			dx = dy = vx = vy = 0;
		}
	}
	else
	{
		if (GetTickCount() - auto_walk_start > autoInfo.timeMove)
		{
			auto_walk_start = 0;
			autoWalk = false;
			dx = dy = vx = vy = 0;
		}
		else
		{
			// Calculate the dx, dy while moving automatically
			vx = autoInfo.vx;
			vy = autoInfo.vy;
			dx = vx * dt;
			dy = vy * dt;
		}
	}
	
}

void CSimon::ProceedOverlapping()
{
	LPGAMEOBJECT ovObj;
	for (UINT i = 0; i < ovObjects.size(); i++)
	{
		ovObj = ovObjects[i];

		if (dynamic_cast<ItemBigHeart*>(ovObj))
		{
			DebugOut(L"[ITEMS] Heart Collected \n");
			heart_quantity += 5;
			ovObj->SetVisible(false);
		}

		else if (dynamic_cast<ItemSmallHeart*>(ovObj))
		{
			DebugOut(L"[ITEMS] Heart Collected \n");
			heart_quantity += 1;
			ovObj->SetVisible(false);
		}

		else if (dynamic_cast<ItemChain*>(ovObj))
		{

			this->powerUp = true;
			this->SetState(SIMON_STATE_IDLE);
			ovObj->SetVisible(false);
			this->whip->PowerUp();
			//DebugOut(L"[INFO] WHIP UPGRADED \n");
		}			

		else if (dynamic_cast<ItemDagger*>(ovObj))
		{
			DebugOut(L"[ITEMS] Dagger Collected \n");
			ovObj->SetVisible(false);
			this->currentSubWeapon = int(SubWeapon::DAGGER);
		}

		else if (dynamic_cast<ItemBoomerang*>(ovObj))
		{
			DebugOut(L"[ITEMS] Boomerang Collected \n");
			ovObj->SetVisible(false);
			this->currentSubWeapon = int(SubWeapon::BOOMERANG);
		}

		else if (dynamic_cast<ItemHolyWater*>(ovObj))
		{
			DebugOut(L"[ITEMS] Holy water Collected \n");
			ovObj ->SetVisible(false);
			this->currentSubWeapon = int(SubWeapon::HOLYWATER);
		}

		else if (dynamic_cast<ItemAxe*>(ovObj))
		{
			DebugOut(L"[ITEMS] Axe Collected \n");
			ovObj->SetVisible(false);
			this->currentSubWeapon = int(SubWeapon::AXE);			
		}

		else if (dynamic_cast<ItemWatch*>(ovObj))
		{
			DebugOut(L"[ITEMS] Stop watch Collected \n");
ovObj->SetVisible(false);
this->currentSubWeapon = int(SubWeapon::STOP_WATCH);
		}

		else if (dynamic_cast<ItemMoneyBagRed*>(ovObj))
		{
		DebugOut(L"[ITEMS] Money Bag Collected \n");
		ovObj->SetVisible(false);
		PointEffects::GetInstance()->ShowPoint(POINT_EFFECT_100, x + 32, y + 5);
		}

		else if (dynamic_cast<ItemMoneyBagPurple*>(ovObj))
		{
		DebugOut(L"[ITEMS] Money Bag Collected \n");
		ovObj->SetVisible(false);
		PointEffects::GetInstance()->ShowPoint(POINT_EFFECT_400, x + 32, y + 5);
		}

		else if (dynamic_cast<ItemMoneyBagYellow*>(ovObj))
		{
		DebugOut(L"[ITEMS] Money Bag Collected \n");
		ovObj->SetVisible(false);
		PointEffects::GetInstance()->ShowPoint(POINT_EFFECT_700, x + 32, y + 5);
		}


	}
}

CSimon::CSimon(float x, float y) :CGameObject()
{
	start_x = x;
	start_y = y;

	this->x = x;
	this->y = y;

	this->heart_quantity = 5;

	SetState(SIMON_STATE_IDLE);

	this->currentSubWeapon = int(SubWeapon::UNKNOWN);
	whip = new CWhip();
}

void CSimon::Update(DWORD dt, vector <LPGAMEOBJECT>* coObjects)
{
	// Calculate x,y
	CGameObject::Update(dt);

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

	if (ovObjects.size() != 0)
	{
		ProceedOverlapping();
	}

	// Being On Stairs
	if (onStairs != 0)
	{
		ProceedOnStairs();
		vy = 0;
	}

	// Auto Walk
	if (autoWalk)
		ProceedAutoWalk();

	// Checking subweapon
	if (subWeapon)
	{
		if (currentSubWeapon ==(int) SubWeapon::STOP_WATCH)
		{
			using_stop_watch = true;
		}
		else
		{
			if (heart_quantity <= 0) subWeapon = false;

			if (lastFrameAttack())
			{
				heart_quantity -= 1;
				this->weapons->Select(int(currentSubWeapon));
				subWeapon = false;
			}
		}
		
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

		x += min_tx * dx + nx * 0.2f;
		y += min_ty * dy + ny * 0.2f;

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

			else if (dynamic_cast<CBoomerang*>(e->obj))
			{
				if (e->nx != 0 || e->ny != 0)
				{
					e->obj->SetVisible(false);
					CBoomerang* bm = dynamic_cast<CBoomerang*> (e->obj);
					bm->SetTurnOver(false);
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
					heart_quantity += 5;
					e->obj->SetVisible(false);
				}
			}

			else if (dynamic_cast<ItemSmallHeart*>(e->obj))
			{
				DebugOut(L"[ITEMS] Heart Collected \n");
				if (e->nx != 0 || e->ny != 0)
				{
					heart_quantity += 1;
					y = y - 0.2f;
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
					//DebugOut(L"[INFO] WHIP UPGRADED \n");
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
					this->currentSubWeapon = int(SubWeapon::DAGGER);
				}
			}

			else if (dynamic_cast<ItemBoomerang*>(e->obj))
			{
				DebugOut(L"[ITEMS] Boomerang Collected \n");
				if (e->nx != 0 || e->ny != 0)
				{
					y = y -0.4f;
					e->obj->SetVisible(false);
					this->currentSubWeapon = int(SubWeapon::BOOMERANG);
				}
			}

			else if (dynamic_cast<ItemHolyWater*>(e->obj))
			{
				DebugOut(L"[ITEMS] Holy water Collected \n");
				if (e->nx != 0 || e->ny != 0)
				{
					e->obj->SetVisible(false);
					this->currentSubWeapon = int(SubWeapon::HOLYWATER);
				}
			}

			else if (dynamic_cast<ItemAxe*>(e->obj))
			{
			DebugOut(L"[ITEMS] Axe Collected \n");
			if (e->nx != 0 || e->ny != 0)
			{
				e->obj->SetVisible(false);
				this->currentSubWeapon = int(SubWeapon::AXE);
			}
			}

			else if (dynamic_cast<ItemWatch*>(e->obj))
			{
			DebugOut(L"[ITEMS] Stop watch Collected \n");
			if (e->nx != 0 || e->ny != 0)
			{
				y = y - 0.4f;
				e->obj->SetVisible(false);
				this->currentSubWeapon = int(SubWeapon::STOP_WATCH);
			}
			}

			
			else if (dynamic_cast<ItemMoneyBagRed*>(e->obj))
			{
				DebugOut(L"[ITEMS] Money Bag Collected \n");
				if (e->nx != 0 || e->ny != 0)
				{
					e->obj->SetVisible(false);	
					PointEffects::GetInstance()->ShowPoint(POINT_EFFECT_100, this->x+ 32, this->y+ 10);			
				}
			}	

			else if (dynamic_cast<ItemMoneyBagPurple*>(e->obj))
			{
				DebugOut(L"[ITEMS] Money Bag Collected \n");
				if (e->nx != 0 || e->ny != 0)
				{
				e->obj->SetVisible(false);
				PointEffects::GetInstance()->ShowPoint(POINT_EFFECT_400, this->x + 32, this->y + 10);
				}
			}

			else if (dynamic_cast<ItemMoneyBagYellow*>(e->obj))
			{
				DebugOut(L"[ITEMS] Money Bag Collected \n");
				if (e->nx != 0 || e->ny != 0)
				{
				e->obj->SetVisible(false);
				PointEffects::GetInstance()->ShowPoint(POINT_EFFECT_700, this->x + 32, this->y + 10);
				}
			}

			else if (dynamic_cast<ItemCrown*>(e->obj))
			{
				DebugOut(L"[ITEMS] (+2000) \n");
				if (e->nx != 0 || e->ny != 0)
				{
				e->obj->SetVisible(false);
				PointEffects::GetInstance()->ShowPoint(POINT_EFFECT_2000, e->obj->x + 5, e->obj->y + 2);
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

			else if (dynamic_cast<CRaven*>(e->obj))
			{
			if ((e->nx != 0 || e->ny != 0) && untouchable == 0)
			{
				e->obj->SetVisible(false);
				if (onStairs == 0)
				{
					StartUntouchable();
					DebugOut(L"[INFO] Enemies collision, Simon is Damaged \n");

					//this->nx = (e->nx != 0) ?	-(e->nx) :	-(e->obj->GetOrientation());

					if (e->nx != 0)
						this->nx = -(e->nx);
					else
						this->nx = -(e->obj->GetOrientation());

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

			else if (dynamic_cast<CBat*>(e->obj) ||
			dynamic_cast<CBlack_Knight*>(e->obj) || 
			dynamic_cast<CHunchBack*>(e->obj) || 
			dynamic_cast<CPhantomBat*>(e->obj))
			{

			if ((e->nx != 0 || e->ny != 0) && untouchable == 0)
			{

				if (onStairs == 0)
				{
					StartUntouchable();
					DebugOut(L"[INFO] Enemies collision, Simon is Damaged \n");

					//this->nx = (e->nx != 0) ?	-(e->nx) :	-(e->obj->GetOrientation());

					if (e->nx != 0)
						this->nx = -(e->nx);
					else
						this->nx = -(e->obj->GetOrientation());

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

	// Clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
	}

	// Check collsion when simon attacking
	if (state == SIMON_STATE_ATTACK || state == SIMON_STATE_SIT_ATTACK)
	{
		whip->SetOrientation(nx);
		whip->SetWhipPosition(D3DXVECTOR2(x, y), isStanding);

		if (lastFrameAttack()) // Only check collsion at the last frame of the whip
		{
			whip->Update(dt, coObjects);
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
		subWeapon = false;
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
		animation_set->at(SIMON_ANI_SIT_ATTACK)->Reset();

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
		animation_set->at(SIMON_ANI_ATTACK)->Reset();

		animation_set->at(SIMON_ANI_SIT_ATTACK)->Reset();
		animation_set->at(SIMON_ANI_SIT_ATTACK)->SetAniStartTime(GetTickCount());
		break;
	}

	case SIMON_STATE_THROW:
	{
		vx = 0;
		subWeapon = true;

		animation_set->at(SIMON_ANI_ATTACK)->Reset();
		animation_set->at(SIMON_ANI_ATTACK)->SetAniStartTime(GetTickCount());

		animation_set->at(SIMON_ANI_ATTACK_UPSTAIR)->Reset();
		animation_set->at(SIMON_ANI_ATTACK_UPSTAIR)->SetAniStartTime(GetTickCount());

		animation_set->at(SIMON_ANI_ATTACK_DOWNSTAIR)->Reset();
		animation_set->at(SIMON_ANI_ATTACK_DOWNSTAIR)->SetAniStartTime(GetTickCount());
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
	onMovingPlatform = 0;
	SetState(SIMON_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

bool CSimon::lastFrameAttack()
{
	if (animation_set->at(SIMON_ANI_ATTACK)->GetCurrentFrame() == 2 ||
		animation_set->at(SIMON_ANI_SIT_ATTACK)->GetCurrentFrame() == 2||
		animation_set->at(SIMON_ANI_ATTACK_UPSTAIR)->GetCurrentFrame() == 2 ||
		animation_set->at(SIMON_ANI_ATTACK_DOWNSTAIR)->GetCurrentFrame() == 2)
		return true;
	else
		return false;
}

void CSimon::Render()
{
	int ani = -1;

	if (state == SIMON_STATE_DIE)
	{
		ani = SIMON_ANI_IDLE;
	}
	else if (state == SIMON_STATE_ATTACK || state == SIMON_STATE_THROW && currentSubWeapon != (int)SubWeapon::STOP_WATCH)
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
	//RenderBoundingBox();	

	// Whip rendering
	if (state == SIMON_STATE_ATTACK || state == SIMON_STATE_SIT_ATTACK)
	{
		whip->Render(animation_set->at(ani)->GetCurrentFrame());
	}
}

void CSimon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x+SIMON_BBOX_WIDTH/2;
	top = y;
	right = left + SIMON_BBOX_WIDTH;
	bottom = top + SIMON_BBOX_HEIGHT;
}