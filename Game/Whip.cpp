#include "Whip.h"
#include "Candle.h"
#define WHIP_ANI_SET 30

CWhip::CWhip():CGameObject()
{	
	SetState(LONG_CHAIN);
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving )
{
	SetDamage(state);

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT temp = coObjects->at(i);
		if (this->IsOverlapping(temp))
		{
			if (dynamic_cast<CCandle*>(temp))
			{
				DebugOut(L"[INFO] Candle destroyed  \n");
				temp->SetState(CANDLE_DESTROYED);
				temp->animation_set->at(CANDLE_DESTROYED)->SetAniStartTime(GetTickCount());
			}

			else if (dynamic_cast<CBreakWall*>(temp))
			{
				DebugOut(L"[INFO] Whip Collision with BreakWall \n");
				CBreakWall* breakwall = dynamic_cast<CBreakWall*>(temp);
				breakwall->Destroy();
			}

			else if (dynamic_cast<CBlack_Knight*>(temp)||
				dynamic_cast<CBat*>(temp)||
				dynamic_cast<CHunchBack*>(temp)||
				dynamic_cast<CSkeleton*>(temp) ||
				dynamic_cast<CGhost*>(temp))
			{
				DebugOut(L"[INFO] Enemy take damaged \n");
				temp->TakeDamage(this->damage);
				float l, t, r, b;
				temp->GetBoundingBox(l, t, r, b);
				hitEffects.push_back({ (l + r) / 2, (t + b) / 2 });				
				
			}
		}
		
	}
}

void CWhip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{	 
	top = y + OFFSET_TOP;
	bottom = top + WHIP_BBOX_HEIGHT;
	switch (state)
	{
	case NORMAL_WHIP:
	{
		if (nx > 0) left = x + OFFSET_RIGHT_CHAIN;
		else left = x + OFFSET_LEFT_SHORT_CHAIN;
		right = left + SHORT_CHAIN_BBOX_WIDTH;
		break;
	}

	case SHORT_CHAIN:
	{
		if (nx > 0) left = x + OFFSET_RIGHT_CHAIN;
		else left = x + OFFSET_LEFT_SHORT_CHAIN;
		right = left + SHORT_CHAIN_BBOX_WIDTH;
		break;
	}

	case LONG_CHAIN:
	{
		if (nx > 0) left = x + OFFSET_RIGHT_CHAIN;
		else left = x + OFFSET_LEFT_LONG_CHAIN;
		right = left + LONG_CHAIN_BBOX_WIDTH;
		break;
	}		
	}
}


void CWhip::SetWhipPosition(D3DXVECTOR2 simonPos, bool isStanding)
{	
	if (nx>0)
	{
		simonPos.x -= 42.0f;
		if(isStanding) simonPos.y -= 6.0f;
		simonPos.y += 7.0f;		
	}
	else
	{
		simonPos.x -= 49.0f;
		if (isStanding) simonPos.y -= 0.0f;
		else simonPos.y += 7.0f;
	}
	
	SetPosition(simonPos.x, simonPos.y);
}

void CWhip::PowerUp()
{
	if (state == NORMAL_WHIP) SetState(SHORT_CHAIN) ;	
	 else if (state == SHORT_CHAIN) SetState(LONG_CHAIN);

	DebugOut(L"[INFO] Whip Updated, level %d\n", GetState());
}

void CWhip::ShowHitEffect()
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

CWhip* CWhip::__instance = NULL;
CWhip* CWhip::GetInstance()
{
	if (__instance == NULL) __instance = new CWhip();
	return __instance;
}

void CWhip::Render(int currentFrame)
{
	CAnimationSets::GetInstance()->Get(WHIP_ANI_SET)->at(state)->RenderByFrame(currentFrame, nx, x, y);
	ShowHitEffect();
	//RenderBoundingBox();
}

void CWhip::SetState(int state)
{
	this->state = state;
}

void CWhip::SetDamage(int state)
{
	if (state == NORMAL_WHIP)
	{
		damage = 1;
	}
	else damage = 2;
}