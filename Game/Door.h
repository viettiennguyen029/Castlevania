#pragma once
#include "GameObject.h"
class CDoor:public CGameObject
{
public:
	CDoor(float l, float t, float r, float b);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

