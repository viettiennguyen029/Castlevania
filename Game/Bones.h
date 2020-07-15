#pragma once
#include "GameObject.h"

#define BONES_BBOX_WIDTH		15
#define BONES_BBOX_HEIGHT	16
class CBones : public CGameObject
{
public:
	CBones();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

};

