#include "Candle.h"

CCandle::CCandle()
{
;
}


void CCandle::Render()
{
	animation_set->at(state)->Render(x, y, nx);
	//The order of the state is correctly defined as the push in order of the animaions.
	RenderBoundingBox();
}


void CCandle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	// Continue here
	left = x;
	top = y;
	right = x + CANDLE_BBOX_WIDTH;
	bottom = y + CANDLE_BBOX_HEIGHT;
}
