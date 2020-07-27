#include "HolyWater.h"
#include "Candle.h"

CHolyWater* CHolyWater::__instance = NULL;

CHolyWater::CHolyWater()
{
	SetVisible(false);
	this->damage = 2;
	vy = HOLY_WATER_VY;	
}

CHolyWater* CHolyWater::GetInstance()
{
	if (__instance == NULL)__instance = new CHolyWater();
	return __instance;
}

void CHolyWater::ShowHitEffect()
{
	if (hitEffects.size() > 0)
	{
		if (startShow == 0)
		{
			startShow = GetTickCount();
		}

		else if (GetTickCount() - startShow > 100)
		{
			startShow = 0;
			hitEffects.clear();
		}

		// rendering hit effect based on the coordinate vector
		for (auto coord : hitEffects)
			hitEffect->Render(coord[0], coord[1], -1);
	}
}

void CHolyWater::Render()
{
	ShowHitEffect();
	animation_set->at(state)->Render(x, y, nx);
}

void CHolyWater::SetState(int state)
{
	CGameObject::SetState(state);
	
}

void CHolyWater::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving )
{
	CGameObject::Update(dt);
	
	
	if (burning_start != 0)
	{
		if (GetTickCount() - burning_start > BURNING_TIME)
		{
			burning = false;
			burning_start = 0;
			this->SetVisible(false);
			SetState(HOLY_WATER_STATE_NORMAL);
		}
	}
		

	// Falling down
	if (burning == false)
	{
		vx = (nx > 0) ?
			HOLY_WATER_SPEED_VX :
			-HOLY_WATER_SPEED_VX;
		vy += HOLY_WATER_VY * this->dt;
	}
		

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx, rdy;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.2f;
		y += min_ty * dy + ny * 0.2f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CCandle*>(e->obj))
			{
				e->obj->SetState(CANDLE_DESTROYED);
				e->obj->animation_set->at(CANDLE_DESTROYED)->SetAniStartTime(GetTickCount());
			}

			else if (dynamic_cast<CBrick*> (e->obj))
			{				
				vx = vy = 0;
				SetState(HOLY_WATER_STATE_BURN);
				if (!burning)
				{
					x += (HOLY_WATER_NORMAL_BBOX_WIDTH - HOLY_WATER_BURN_BBOX_WIDTH) ;
					y += (HOLY_WATER_NORMAL_BBOX_HEIGHT - HOLY_WATER_BURN_BBOX_HEIGHT)-1;

					// Start burning on the ground
					burning = true;
					burning_start = GetTickCount();
				}
			}

			else if (dynamic_cast<CHunchBack*> (e->obj) || 
				dynamic_cast<CSkeleton*> (e->obj) || 
				dynamic_cast<CRaven*> (e->obj) ||
				dynamic_cast<CGhost*> (e->obj) ||
				dynamic_cast<CBossBat*>(e->obj) ||
				dynamic_cast<CZombie*> (e->obj))
			{
				e->obj->TakeDamage(this->damage);
				float l, t, r, b;
				e->obj->GetBoundingBox(l, t, r, b);
				hitEffects.push_back({ (l + r) / 2, (t + b) / 2 });
			}

		}

		// clean up collision events
		for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}
	


void CHolyWater::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	switch (state)
	{
	case HOLY_WATER_STATE_NORMAL:
	{
		right = x + HOLY_WATER_NORMAL_BBOX_WIDTH;
		bottom = y + HOLY_WATER_NORMAL_BBOX_HEIGHT;
		break;
	}

	case HOLY_WATER_STATE_BURN:
	{
		right = x + HOLY_WATER_BURN_BBOX_WIDTH;
		bottom = y + HOLY_WATER_BURN_BBOX_HEIGHT;
		break;
	}
	}
}

