#include "Whip.h"
#include "Candle.h"
#define WHIP_ANI_SET 36

CWhip::CWhip():CGameObject()
{
	//SetState(NORMAL_WHIP);
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	SetDamage();

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT temp = coObjects->at(i);
		if (dynamic_cast<CCandle*>(temp))
		{
			CCandle* candle = dynamic_cast<CCandle*> (temp);
			if (this->IsOverlapping(temp))
			{
				DebugOut(L"[INFO]Whip Collision with Torch \n");
				temp->SetState(CANDLE_DESTROYED);
				temp->animation_set->at(CANDLE_DESTROYED)->SetAniStartTime(GetTickCount());
			}
		}

		else  if (dynamic_cast<CBreakWall*>(temp))
		{
			CBreakWall* breakwall = dynamic_cast<CBreakWall*>(temp);
			if (this->IsOverlapping(temp))
			{
				DebugOut(L"[INFO] Whip Collision with BreakWall \n");
				breakwall->Destroy();
			}
		}

		else if (dynamic_cast<CBlack_Knight*>(temp))
		{

			if (this->IsOverlapping(temp))
			{
				DebugOut(L"[INFO] Whip Collision with Knight \n");
				temp->TakeDamage(this->damage);
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

bool CWhip::isColliding(float obj_left, float obj_top, float obj_right, float obj_bottom)
{
	float whip_left, whip_top, whip_right, whip_bottom;
	GetBoundingBox(whip_left, whip_top, whip_right, whip_bottom);
	
	return CGameObject::AABB(whip_left, whip_top, whip_right, whip_bottom, obj_left, obj_top, obj_right, obj_bottom);
}

void CWhip::SetWhipPosition(D3DXVECTOR2 simonPos, bool isStanding)
{	
	if (nx>0)
	{
		simonPos.x -= 49.0f;
		if(isStanding) simonPos.y -= 3.0f;
		else simonPos.y += 3.0f;		
	}
	else
	{
		simonPos.x -= 53.0f;
		if (isStanding) simonPos.y -= 3.0f;
		else simonPos.y += 3.0f;
	}
	
	SetPosition(simonPos.x, simonPos.y);
}

void CWhip::PowerUp()
{
	if (state == NORMAL_WHIP) SetState(SHORT_CHAIN) ;	
	 else if (state == SHORT_CHAIN) SetState(LONG_CHAIN);

	DebugOut(L"Whip Level %d\n", GetState());
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
	// RenderBoundingBox();
}

void CWhip::SetState(int state)
{
	this->state = state;
}

void CWhip::SetDamage()
{
	if (state == NORMAL_WHIP)
	{
		damage = 1;
	}
	else damage = 3;
}

