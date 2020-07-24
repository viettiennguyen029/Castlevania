#pragma once
#include "GameObject.h"

#define BONE_BBOX_WIDTH	15
#define BONE_BBOX_HEIGHT	16

#define BONE_FLY_SPEED_VX 0.003f
class CBone : public CGameObject
{
	bool stop;
	float start_x;
	float end_y;
public:	
	CBone(float x, float y,int nx);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL, bool stopMoving=false);
	virtual void Render();

	void SetStart_x(float strat_x) { this->start_x = strat_x; }
	void SetEnd_y(float end_y) { this->end_y = end_y; }
};

