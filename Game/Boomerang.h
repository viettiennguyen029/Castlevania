#pragma once
#include "GameObject.h"

#define BOOMERANG_BBOX_WIDTH		16
#define BOOMERANG_BBOX_HEIGHT	12

class CBoomerang: public CGameObject
{
	DWORD turnoverDelayTime=0;
	bool turnOver = false;
public:
	CBoomerang();
	void SetTurnOver(bool turnOver) { this->turnOver = turnOver; }
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

