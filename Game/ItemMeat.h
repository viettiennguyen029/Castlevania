#pragma once
#include "GameObject.h"

#define ITEM_MEAT_BBOX_WIDTH		16
#define ITEM_MEAT_BBOX_HEIGHT	13

class ItemMeat : public CGameObject
{
	DWORD start_visible = 0;
public:
	ItemMeat();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving = false);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

