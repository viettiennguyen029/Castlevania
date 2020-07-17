#pragma once
#include "GameObject.h"

#define BONES_BBOX_WIDTH		15
#define BONES_BBOX_HEIGHT	16

#define BONES_FLY_SPEED_VX 0.0035f
class CBones : public CGameObject
{
	static CBones* __instance;
	float start_x;
	float end_y;
public:	
	CBones();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	void SetStart_x(float strat_x) { this->start_x = strat_x; }
	void SetEnd_y(float end_y) { this->end_y = end_y; }

	static CBones* GetInstance();
};

