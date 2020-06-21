#pragma once
#include "GameObject.h"

#define ZOMBIE_MOVING_SPEED	0.05f

#define ZOMBIE_STATE_MOVING	0

#define ZOMBIE_BBOX_WIDTH	16
#define ZOMBIE_BBOX_HEIGHT	32

class CZombie : public CGameObject
{
public:
	CZombie();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();	
	virtual void SetState(int state);
};

