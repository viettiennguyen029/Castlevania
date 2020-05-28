#pragma once
#include "GameObject.h"
#define DAGGER_SPEED				0.17f

#define DAGGER_BBOX_WIDTH		16
#define DAGGER_BBOX_HEIGHT		10

class CDagger: public CGameObject
{
public:
	CDagger();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};