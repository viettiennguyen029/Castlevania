#include "Whip.h"
#include "Candle.h"

CWhip::CWhip():CGameObject()
{
	SetState(NORMAL_WHIP);
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	// SetWhipPosition(x,y);
}

bool CWhip::isColliding(float obj_left, float obj_top, float obj_right, float obj_bottom)
{
	float whip_left, whip_top, whip_right, whip_bottom;
	GetBoundingBox(whip_left, whip_top, whip_right, whip_bottom);
	
	return CGameObject::AABB(whip_left, whip_top, whip_right, whip_bottom, obj_left, obj_top, obj_right, obj_bottom);
}

void CWhip::SetWhipPosition(float x, float y)
{
	float xW, yW;
	if (nx > 0)
	{
		xW =x- 8.0f;
		yW =y+ 4.0f;	

	}
	else 
	{
		xW =x+22.0f;
		yW= y+4.0f;
	}
	

	SetPosition(xW, yW);
}
	
void CWhip::PowerUp()
{
	if (state == NORMAL_WHIP) SetState(SHORT_CHAIN);
	else if (state == SHORT_CHAIN) SetState(LONG_CHAIN);
}

void CWhip::Render(int currentFrame)
{	
	if (0 <= currentFrame < 2)
	{
		CAnimationSets::GetInstance()->Get(4)->at(state)->RenderByFrame(currentFrame, nx, x, y);
	}
	else
	{
		return;
	}
	RenderBoundingBox();
}

void CWhip::SetState(int state)
{

}

void CWhip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	top = y ;
	bottom = top + WHIP_BBOX_HEIGHT;
	if (state == NORMAL_WHIP)
	{
		left = x;
		right = x + WHIP_BBOX_WIDTH;
	}
	
}