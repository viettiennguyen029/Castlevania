#include "Axe.h"
#include "Candle.h"

CAxe* CAxe::__instance = NULL;
CAxe::CAxe():CGameObject()
{
	SetVisible(false);
	this->damage = 2;
	SetState(0);
}

void CAxe::Render()
{
	animation_set->at(state)->Render(x, y, nx);
	ShowHitEffect();
}

void CAxe::ShowHitEffect()
{
	if (hitEffects.size() > 0)
	{
		if (startShow == 0)
		{
			startShow = GetTickCount();
		}

		else if (GetTickCount() - startShow > HIT_EFFECT_LIFE_SPAN)
		{
			startShow = 0;
			hitEffects.clear();
		}

		// rendering hit effect based on the coordinate vector
		for (auto coord : hitEffects)
			hitEffect->Render(coord[0], coord[1], -1);
	}
}

void CAxe::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving )
{
	CGameObject::Update(dt);

	// Falling down
	vy += AXE_GRAVITY * dt;

	float start_x = CSimon::GetInstance()->x;

	if (abs(x - start_x) <= 12)
	{
		vx = (nx > 0) ? AXE_FLY_SPEED_VX*dt : -AXE_FLY_SPEED_VX*dt;		
		vy = -AXE_FLY_SPEED_VY;
	}
	else 
	{		
		vx = (nx > 0) ? AXE_FLY_SPEED_VX * dt : -AXE_FLY_SPEED_VX * dt;
		if (abs(x - start_x) >= 70)
			vy = AXE_FLY_SPEED_VY;
	}


	if (!IsInViewport())
	{
		SetVisible(false);
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

		x += min_tx * dx;
		y += min_ty * dy ;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			// collision with candle
			if (dynamic_cast<CCandle*>(e->obj))
			{
				e->obj->SetState(CANDLE_DESTROYED);
				e->obj->animation_set->at(CANDLE_DESTROYED)->SetAniStartTime(GetTickCount());
			}

			else if (dynamic_cast<CBat*>(e->obj) ||
				dynamic_cast<CBlack_Knight*>(e->obj) ||
				dynamic_cast<CRaven*>(e->obj) ||
				dynamic_cast<CHunchBack*>(e->obj) ||
				dynamic_cast<CGhost*>(e->obj) ||
				dynamic_cast<CBossBat*>(e->obj) ||
				dynamic_cast<CZombie*>(e->obj))
			{
				if (e->nx != 0 || e->ny != 0)
				{
					vx = 0;
					e->obj->TakeDamage(this->damage);
					float l, t, r, b;
					e->obj->GetBoundingBox(l, t, r, b);
					hitEffects.push_back({ (l + r) / 2, (t + b) / 2 });
				}
			}
			
			
		}
	}

	// Clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CAxe::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + AXE_BBOX_WIDTH;
	bottom = y + AXE_BBOX_HEIGHT;
}

CAxe* CAxe::GetInstance()
{
	if (__instance == NULL)__instance = new CAxe();
	return __instance;
}