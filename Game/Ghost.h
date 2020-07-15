#pragma once
#include "GameObject.h"

#define GHOST_STATE_MOVING	 0

#define GHOST_BBOX_WIDTH	16
#define GHOST_BBOX_HEIGHT	16

#define GHOST_MOVING_SPEED 0.052f
class CGhost : public CGameObject
{
private:
	static CGhost* __instance;
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	CGhost();
	virtual void SetState(int state);
	static CGhost* GetInstance();
};

