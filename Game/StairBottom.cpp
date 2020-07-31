#include "StairBottom.h"

CStairBottom::CStairBottom()
{
}

void CStairBottom::Render()
{
	//RenderBoundingBox();
}

void CStairBottom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + STAIR_BBOX_WIDTH;
	b = t + STAIR_BBOX_HIEGHT;
}
