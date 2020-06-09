#pragma once
#include "GameObject.h"
class CVariousStair:public CGameObject
{
public:
	CVariousStair(float l, float t, float r, float b);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

