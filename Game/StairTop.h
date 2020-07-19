#pragma once
#include "GameObject.h"

#define STAIR_BBOX_WIDTH	16
#define STAIR_BBOX_HIEGHT	30

class CStairTop : public CGameObject
{
public:
	CStairTop();
	virtual void Render() { RenderBoundingBox(); }
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};