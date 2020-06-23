#pragma once
#include "GameObject.h"
#define HUNCH_BACK_STATE_IDLE			0
#define HUNCH_BACK_STATE_JUMP		1

#define HUNCH_BACK_BBOX_HEIGHT	16
#define HUNCH_BACK_BBOX_WIDTH		16

class CHunchBack  : public CGameObject
{
public:
	CHunchBack();
	virtual void SetState(int state);
	virtual void Render();	
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);	
};