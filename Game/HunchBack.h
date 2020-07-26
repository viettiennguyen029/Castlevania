#pragma once
#include "GameObject.h"
#define HUNCH_BACK_STATE_IDLE			0
#define HUNCH_BACK_STATE_JUMP		1

#define HUNCH_BACK_BBOX_HEIGHT	16
#define HUNCH_BACK_BBOX_WIDTH		16

#define SMALL_HOPPING_VX		0.16f
#define SMALL_HOPPING_VY		0.08f

#define LARGE_HOPPING_VX		0.1f
#define LARGE_HOPPING_VY		 0.36f

#define HUNCH_BACK_GRAVITY			0.0006
#define  LARGE_HOPPING_GRAVITY	0.0009

#define DISTANCE_ACTIVE_HUNCH_BACK 112
#define HUNCH_BACK_WATING_TIME	500

class CHunchBack  : public CGameObject
{
	bool stop = false;
	bool hopping = false;
	bool grounding = false;
	DWORD startWaitingTime = 0;
public:
	CHunchBack();

	void DetectPlayer();
	void Hopping();
	void LargeHopping();

	virtual void SetState(int state);
	virtual void Render();	
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL, bool stopMoving = false);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);	
};