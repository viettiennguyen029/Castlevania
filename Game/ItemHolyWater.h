#pragma once
#include "GameObject.h"

#define ITEM_HOLY_WATER_BBOX_WIDTH		16
#define ITEM_HOLY_WATER_BBOX_HEIGHT	16

class ItemHolyWater: public CGameObject
{
	DWORD start_visible = 0;
public:
	ItemHolyWater();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving = false);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

