#pragma once
#include "GameObject.h"
#define HUNCH_BACK_STATE_IDLE			0
#define HUNCH_BACK_STATE_JUMP		1
#define HUNCH_BACK_STATE_ON_GROUND	2

#define HUNCH_BACK_BBOX_HEIGHT	16
#define HUNCH_BACK_BBOX_WIDTH		16

#define HUNCHBACK_GRAVITY		0.00042f
#define HUNCH_BACK_JUMP_SPEED_X	0.08f

class CHunchBack  : public CGameObject
{
	bool hopping = false;
	DWORD startWaitingTime = 0;
public:
	CHunchBack();
	virtual void SetState(int state);
	virtual void Render();	
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);	
};