#pragma once
#include "GameObject.h"

#define GHOST_STATE_MOVING	 0
#define GHOST_BBOX_WIDTH	16
#define GHOST_BBOX_HEIGHT	16

class CGhost : public CGameObject
{
	float start_x, start_y;
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	CGhost(float x = 0.0f, float y = 0.0f);
	virtual void SetState(int state);
};

