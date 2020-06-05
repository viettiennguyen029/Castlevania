#include "StairTop.h"

CStairTop::CStairTop(float l, float t, float r, float b)
{
	x = l;
	y = t;
	width = r - l + 1;
	height = b - t + 1;
}
void CStairTop::Render()
{
	RenderBoundingBox();
}

void CStairTop::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}


