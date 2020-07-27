#include "PhantomBat.h"
#include "Simon.h"

bool CPhantomBat::IsActive()
{
	if (player_x > boss_x)
		return true;

	return false;
}

bool CPhantomBat::IsInRetreats()
{
	// Check if boss has reached to the retreat point or not
	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);

	if (left < retreats_x && right > retreats_x) vx = 0;
	if (top < retreats_y && bottom > retreats_y) vy = 0;

	if (left < retreats_x && right > retreats_x && top < retreats_y && bottom > retreats_y)
		return true;

	return false;
}

void CPhantomBat::GetBossPosition()
{
	boss_x = x+PHANTOM_BAT_BBOX_WIDTH/2;
	boss_y = y + PHANTOM_BAT_BBOX_HEIGHT / 2;
}

void CPhantomBat::DetectPlayer()
{
	float left, top, right, bottom;
	CSimon::GetInstance()->GetBoundingBox(left, top, right, bottom);
	
	player_x = (left + right) / 2;
	player_y = (top + bottom) / 2;

	//DebugOut(L"Detecting Simon\n");
}

void CPhantomBat::CalcRetreatPoint()
{
	retreats_x = (boss_x + player_x) / 2;
	retreats_y = (boss_y + player_y) / 2;

	// Make sure the Boss have a good distance from Simon ( for good player's experience)
	if (abs(retreats_x - player_x) < PHANTOM_BAT_MIN_DISTANCE_WITH_SIMON)
	{
		if (retreats_x < player_x)
			retreats_x = player_x - PHANTOM_BAT_MIN_DISTANCE_WITH_SIMON;
		else
		{
			retreats_x = player_x + PHANTOM_BAT_MIN_DISTANCE_WITH_SIMON;
		}
	}

	if (abs(retreats_y - player_y) < PHANTOM_BAT_MIN_DISTANCE_WITH_SIMON)
	{
		retreats_y = player_y - PHANTOM_BAT_MIN_DISTANCE_WITH_SIMON;
	}

	// Keep the BossBat being inside viewport
	float left, top, right, bottom;
	CGame::GetInstance()->GetCameraBoundingBox(left, top, right, bottom);

	if (retreats_x > right)		
		retreats_x = player_x - PHANTOM_BAT_MIN_DISTANCE_WITH_SIMON;

	if (retreats_x < left)		
		retreats_x = player_x + PHANTOM_BAT_MIN_DISTANCE_WITH_SIMON;

	if (retreats_y < top || retreats_y > bottom)
		retreats_y = retreats_y - PHANTOM_BAT_MIN_DISTANCE_WITH_SIMON;

	/*if (retreats_y < top || retreats_y > bottom)
		DebugOut(L"Retreat point out of camera !\n");*/

	
}

CPhantomBat::CPhantomBat(float x, float y)
{
	this->start_x = x;
	this->start_y = y;
	SetState(0);
}

void CPhantomBat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + PHANTOM_BAT_BBOX_WIDTH;
	bottom = y + PHANTOM_BAT_BBOX_HEIGHT;
}

void CPhantomBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving )
{
	if(stopMoving)
	{
		stop = true;
		return;
	}
	else
	{
	stop = false;
	}

	CGameObject::Update(dt);

	// Update the current boss's central position
	GetBossPosition();

	// Find player
	DetectPlayer();

	// Active Phantom Bat
	if (inActive)
	{
		if (IsActive())
		{
			inActive = false;
			retreat_start = GetTickCount();
		}
		else
			return;
	}

	/*
		When Phantom Bat is Actived:
		Retreats -> Swoop Down-> Go to Retreats -> Retreats -> (again...)
	*/

	if (retreat_start != 0)
	{
		if (GetTickCount() - retreat_start > PHANTOM_BAT_RETREATS_TIME)
			retreat_start = 0;
	}
	else 
	{
		if (retreats)
			GoToRetreats();

		else
		{
			if (swoopDown_start == 0)
			{
				SwoopDown();
			}
			else
			{
				ProceedAttacking();
			}
		}
	}

	// Keep the BossBat being inside the viewport	

	

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

	float left, top, right, bottom;
		CGame::GetInstance()->GetCameraBoundingBox(left, top, right, bottom);
		if (x < left || x + PHANTOM_BAT_BBOX_WIDTH > right)
			vx = -vx;
		if (y < top +40 || y + PHANTOM_BAT_BBOX_HEIGHT > bottom)
			vx = -vy;
}

void CPhantomBat::Render()
{
	int ani = -1;

	if (inActive==true) ani = 0;
	else ani = 1;

	if (stop)
	{
		int currentFrame = animation_set->at(ani)->GetCurrentFrame();
		animation_set->at(ani)->SetCurrentFrame(currentFrame);
		animation_set->at(ani)->RenderByFrame(currentFrame, nx, x, y);
	}
	else
		animation_set->at(ani)->Render(x, y, nx);

	//RenderBoundingBox();
}

void CPhantomBat::GoToRetreats()
{	
	if (IsInRetreats()) // Check if boss has reached to the retreat point or not
	{
		retreats = false;
		TakeARetreats();
		DebugOut(L"Going to rest point !\n");
	}

	
	//else // Going to the retreat point
	//{
	//	/*vx = (boss_x < retreats_x) ?
	//		PHANTOM_BAT_RETREATS_VX :
	//		-PHANTOM_BAT_RETREATS_VX;

	//	vy = (boss_y < retreats_y) ?
	//		PHANTOM_BAT_RETREATS_VY :
	//		-PHANTOM_BAT_RETREATS_VY;*/
	//}	
}

void CPhantomBat::TakeARetreats()
{
	if (retreat_start == 0)
	{
		retreat_start = GetTickCount();
		vx = vy = 0;
	}
	DebugOut(L"Take a short rest !\n");
}

void CPhantomBat::SwoopDown()
{
	if (swoopDown_start == 0)
	{
		// Vector speed to run into Simon for an attack
		vx = (player_x -boss_x) / (PHANTOM_BAT_SWOOP_DOWN_TIME / 2.5);
		vy = (player_y - boss_y) / (PHANTOM_BAT_SWOOP_DOWN_TIME / 2.5);

		// To keep the boss move not too fast
		vx = (vx > 0) ?
			(vx > PHANTOM_BAT_MAX_SPEED_VX) ? PHANTOM_BAT_MAX_SPEED_VX : vx :
			(vx < -PHANTOM_BAT_MAX_SPEED_VX) ? -PHANTOM_BAT_MAX_SPEED_VX : vx;

		vy = (vy > 0) ?
			(vy > PHANTOM_BAT_MAX_SPEED_VY) ? PHANTOM_BAT_MAX_SPEED_VY : vy :
			(vy < -PHANTOM_BAT_MAX_SPEED_VY) ? -PHANTOM_BAT_MAX_SPEED_VY : vy;

		// Start timer for attacking action
		swoopDown_start = GetTickCount();
		 

	}
}

void CPhantomBat::ProceedAttacking()
{
	// If run out of an attack time
	if (GetTickCount() - swoopDown_start > PHANTOM_BAT_SWOOP_DOWN_TIME)
	{
		// Stop timer
		swoopDown_start = 0;

		// Stop movement
		vx = vy = 0;

		// Going to rest ( will move with lower speed )
		retreats = true;
		CalcRetreatPoint();

		// Assign retreat speed
		vx = (boss_x < retreats_x) ?
			PHANTOM_BAT_RETREATS_VX :
			-PHANTOM_BAT_RETREATS_VX;

		vy = (boss_y < retreats_y) ?
			PHANTOM_BAT_RETREATS_VY :
			-PHANTOM_BAT_RETREATS_VY;
	}

	// Keep decreasing the Boss's vy to create flying -up-movement
	else
		vy += PHANTOM_BAT_BACK_TO_RETREATS_SPEED_VY;
}