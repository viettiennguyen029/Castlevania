#include "Whip.h"
#include "Candle.h"
CWhip::CWhip():CGameObject()
{
	SetState(NORMAL_WHIP);
}

void CWhip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (nx > 0)
	{
		left = x+70;
		top = y+7;
		right = left+ WHIP_BBOX_WIDTH;
		bottom = top+ WHIP_BBOX_HEIGHT;
	}
	else
	{
		left = x;
		top = y;
		right = x - WHIP_BBOX_WIDTH;
		bottom = y + WHIP_BBOX_HEIGHT;
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
	if (isStanding)
	{
		if (nx > 0)
		{
			simonPos.x -= 49.0f;
			simonPos.y -= 3.0f;
		}
		else
		{
			simonPos.x -= 53.0f;
			simonPos.y -= 2.0f;
		}
	}
	
	
	SetPosition(simonPos.x, simonPos.y);
}
	
void CWhip::PowerUp()
{
	if (state == NORMAL_WHIP) SetState(SHORT_CHAIN);
	else if (state == SHORT_CHAIN) SetState(LONG_CHAIN);
}

void CWhip::Render(int currentFrame)
{
	CAnimationSets::GetInstance()->Get(3)->at(NORMAL_WHIP)->RenderByFrame(currentFrame, nx, x, y);
	RenderBoundingBox();
}

void CWhip::SetState(int state)
{

}
