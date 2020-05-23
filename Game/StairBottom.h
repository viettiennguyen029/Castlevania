#pragma once
#include "GameObject.h"

class CStairBottom : public CGameObject
{
	int width;
	int height;

public:
	CStairBottom(float l, float t, float r, float b);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};