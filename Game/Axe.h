#pragma once
#include "GameObject.h"
#define AXE_BBOX_WIDTH		15
#define AXE_BBOX_HEIGHT	14

#define AXE_GRAVITY		0.0012

#define AXE_FLY_SPEED_VX	0.009f
#define AXE_FLY_SPEED_VY		0.3

class CAxe :public CGameObject
{
	static CAxe* __instance;
	bool swinging = true;
	DWORD swinging_start = 0;
public:
	CAxe();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	static CAxe* GetInstance();
};

