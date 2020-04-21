#include "Candle.h"

CCandle::CCandle()
{
;
}


void CCandle::Render()
{
	animation_set->at(0)->Render(x, y, nx);
}


void CCandle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}
