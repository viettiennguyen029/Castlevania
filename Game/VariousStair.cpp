#include "VariousStair.h"

CVariousStair::CVariousStair(float l, float t, float r, float b)
{
	x = l;
	y = t;
	width = r - l + 1;
	height = b - t + 1;
}

void CVariousStair::Render()
{
	RenderBoundingBox();
}

void CVariousStair::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}