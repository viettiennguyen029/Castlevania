#include "Whip.h"

CWhip::CWhip():CGameObject()
{
	SetState(LONG_CHAIN);
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
		if (state != LONG_CHAIN)
			left = x + 50;
		else left = x + 20;
	}
	else if (nx > 0)
	{
		if (state != LONG_CHAIN)
			left = (240 - 50) - WHIP_BBOX_WIDTH + x;
		else left = (240 - 20) - LONG_CHAIN_BBOX_WIDTH + x;
	}

	if (state != LONG_CHAIN)
		right = left + WHIP_BBOX_WIDTH;
	else  right = left + LONG_CHAIN_BBOX_WIDTH;
}

void CWhip::SetWhipPosition(D3DXVECTOR2 simonPos)
{

		SetPosition(simonPos.x, simonPos.y);
}
	
void CWhip::PowerUp()
{
	if (state == NORMAL_WHIP) SetState(SHORT_CHAIN);
	else if (state == SHORT_CHAIN) SetState(LONG_CHAIN);
}

void CWhip::Render(int currentFrame)
{
	if (currentFrame >= 0)
	{
		if (currentFrame == 0)
		{			
			if (nx<0)
				CAnimationSets::GetInstance()->Get(4)->at(NORMAL_WHIP)->RenderByFrame(currentFrame, nx, x +16, y + 7);
			else 
				CAnimationSets::GetInstance()->Get(4)->at(NORMAL_WHIP)->RenderByFrame(currentFrame, nx, x - 6, y + 7);
		}		
		else if (currentFrame == 1)
		{			
			if (nx<0)
			{
				CAnimationSets::GetInstance()->Get(4)->at(NORMAL_WHIP)->RenderByFrame(currentFrame, nx, x +16, y + 7);
			}
			else 
				CAnimationSets::GetInstance()->Get(4)->at(NORMAL_WHIP)->RenderByFrame(currentFrame, nx, x - 16, y + 7);
		}			
		else if (currentFrame == 2)
		{
			if (nx < 0)
			{
				CAnimationSets::GetInstance()->Get(4)->at(NORMAL_WHIP)->RenderByFrame(currentFrame, nx, x - 23, y + 7);
			}
			else 
				CAnimationSets::GetInstance()->Get(4)->at(NORMAL_WHIP)->RenderByFrame(currentFrame, nx, x - 16, y + 7);
		}
			
	}
	
}

void CWhip::SetState(int state)
{

}
