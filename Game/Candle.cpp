#include "Candle.h"

CCandle::CCandle()
{	
}

void CCandle::Render()
{
	/*
		The order of the state is correctly defined as the push in order of the animaions.
	*/
	animation_set->at(state)->Render(x, y, -1);
	// RenderBoundingBox();
}

void CCandle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (state== CANDLE_DESTROYED 
		&& animation_set->at(state)->IsOver(CANDLE_DESTROYED_EFFECT_TIME))
	{
		this->visible = false;		
		// Update items
		CItems::GetInstance()->CheckAndDrop(this);
		return;
	}
}

void CCandle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	switch (state)
	{
	case CANDLE_STATE_BIG:
	{
		right = x + CANDLE_BIG_BBOX_WIDTH;
		bottom = y + CANDLE_BIG_BBOX_HEIGHT;
		break;
	}
	case CANDLE_STATE_SMALL:
	{
		right = x + CANDLE_SMALL_BBOX_WIDTH;
		bottom = y + CANDLE_SMALL_BBOX_HEIGHT;
		break;
	}
	case CANDLE_DESTROYED:
	{
		break;
	}
	}	
}