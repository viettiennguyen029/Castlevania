#pragma once
#include "GameObject.h"
#define BAT_BBOX_WIDTH		16
#define BAT_BBOX_HEIGHT		16

#define BAT_STATE_IDLE								0
#define BAT_STATE_FLYING_DOWN			1
#define BAT_STATE_FLYING_HORIZONTAL  2

#define BAT_FLYING_DOWN_DY		32

#define POINT_ACTIVE_BAT_X			96
#define POINT_ACTIVE_BAT_Y			34
class CBat:public CGameObject
{
	float start_x, start_y; 
public:
	CBat(float x=0.0f,float y=0.0f); 
	virtual void Render();
	virtual void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};


