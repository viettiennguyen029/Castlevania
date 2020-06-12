#pragma once
#include "GameObject.h"

#define ITEM_BOOMERANG_BBOX_WIDTH	16
#define ITEM_BOOMERANG_BBOX_HEIGHT	12

class ItemBoomerang:public CGameObject
{
	DWORD start_visible = 0;
public:
	ItemBoomerang();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

