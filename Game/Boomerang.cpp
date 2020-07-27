#include "Boomerang.h"
#include "Simon.h"
#include "Candle.h"
#define BOOMERANG_VX	0.12f
#define BOOMERANG_MAX_DISTANCE	150

CBoomerang::CBoomerang(): CGameObject()
{
	vx = BOOMERANG_VX;
	this->damage = 2;
}

void CBoomerang::ShowHitEffect()
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

void CBoomerang::Render()
{
	ShowHitEffect();
	animation_set->at(0)->Render(x, y, nx);
}

void CBoomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOOMERANG_BBOX_WIDTH;
	bottom = y + BOOMERANG_BBOX_HEIGHT;
}

void CBoomerang::Update(DWORD dt, vector <LPGAMEOBJECT>* coObjects, bool stopMoving)
{

	float left, top, right, bottom;
	CGame::GetInstance()->GetCameraBoundingBox(left, top, right, bottom);

	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);

	// Boomerang moving logic
	if (!turnOver)
	{
		if (x > (right - BOOMERANG_BBOX_WIDTH) || x <= left) // Collision logic with screen edge
		{
			this->nx = -nx;
			turnOver = true;
		}

		// The limitation of boomerang 
		else if (abs(x - xS) > BOOMERANG_MAX_DISTANCE)
		{
			this->nx = -nx;
			turnOver = true;
		}
	}

	else
	{
		if ((x > right) || (x <= left)) 
		{
			SetVisible(false);
			turnOver = false;
		}
	}

	
	vx = BOOMERANG_VX * nx;

	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size()==0)
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
		y += min_ty * dy;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if ( dynamic_cast<CBat*>(e->obj) || 
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

			else if (dynamic_cast<CCandle*>(e->obj))
			{
				if (e->nx != 0 || e->ny != 0)
				{
					e->obj->SetState(CANDLE_DESTROYED);
					e->obj->animation_set->at(CANDLE_DESTROYED)->SetAniStartTime(GetTickCount());
				}


			}
		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];

}
