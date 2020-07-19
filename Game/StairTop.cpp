#include "StairTop.h"

CStairTop::CStairTop()
{

}

void CStairTop::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + STAIR_BBOX_WIDTH;
	b = t + STAIR_BBOX_HIEGHT;
}