#pragma once
#include "GameObject.h"

#define SKELETON_BBOX_WIDTH	16
#define SKELETON_BBOX_HEIGHT	32

#define SKELETON_STATE_DANCING	0

class CSkeleton : public CGameObject
{
	float start_x;
	float start_y;

	static CSkeleton* __instance;

public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	CSkeleton(float x = 0.0f, float y = 0.0f);
	virtual void SetState(int state);

	static CSkeleton* GetInstance();
};