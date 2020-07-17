#pragma once
#include "GameObject.h"
#define RAVEN_STATE_IDLE			0
#define RAVEN_STATE_FLYING	1

#define RAVEN_BBOX_WIDTH		16
#define RAVEN_BBOX_HEIGHT	16

#define RAVEN_FLYING_DOWN_SPEED	0.05

#define RAVEN_FLYING_DOWN_TIME		400
#define RAVEN_DELAY_TIME	100
class CRaven : public CGameObject
{
	int ny;

	bool attackingPlayer = false;
	DWORD attackingPlayer_start  =0;
	
	bool activeRaven = false;
	DWORD active_raven_start = 0;

	bool buffSpeed = false;
public:
	CRaven();
	virtual void Render();
	virtual void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};