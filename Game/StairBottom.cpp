#include "StairBottom.h"

CStairBottom::CStairBottom(float l, float t, float r, float b)
{
	x = l;
	y = t;
	width = r - l + 1;
	height = b - t + 1;
}

void CStairBottom::Render()
{
	RenderBoundingBox();
}

void CStairBottom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
