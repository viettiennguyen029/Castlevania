#include "Whip.h"
#include "Candle.h"
CWhip::CWhip():CGameObject()
{
	SetState(NORMAL_WHIP);
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
}



void CWhip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	top = y + 15;
	bottom = top + WHIP_BBOX_HEIGHT;
	if (nx < 0)
	{
		left = x + 50;
		right = left + WHIP_BBOX_WIDTH;
	}
	else if (nx > 0)
	{
		left = 190 - WHIP_BBOX_WIDTH + x;
		right = left + WHIP_BBOX_WIDTH;
	}
}

bool CWhip::isColliding(float obj_left, float obj_top, float obj_right, float obj_bottom)
{
	float whip_left, whip_top, whip_right, whip_bottom;
	GetBoundingBox(whip_left, whip_top, whip_right, whip_bottom);
	
	return CGameObject::AABB(whip_left, whip_top, whip_right, whip_bottom, obj_left, obj_top, obj_right, obj_bottom);
}

void CWhip::SetWhipPosition(D3DXVECTOR2 simonPos)
{
	simonPos.x -= 50;
	simonPos.y -= 4;
	SetPosition(simonPos.x, simonPos.y);
}
	
void CWhip::PowerUp()
{
	if (state == NORMAL_WHIP) SetState(SHORT_CHAIN);
	else if (state == SHORT_CHAIN) SetState(LONG_CHAIN);
}

void CWhip::Render(int currentFrame)
{
	
	CAnimationSets::GetInstance()->Get(4)->at(NORMAL_WHIP)->RenderByFrame(currentFrame, nx, x+6, y);
	RenderBoundingBox();
}

void CWhip::SetState(int state)
{

}
