#pragma once
#include "GameObject.h"
#define BAT_BBOX_WIDTH		16
#define BAT_BBOX_HEIGHT		16

#define BAT_STATE_IDLE		0
#define BAT_STATE_FLYING 1
class CBat:public CGameObject
{

public:
	DWORD flyingDownTime=0;
	CBat();
	virtual void Render();
	virtual void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};


