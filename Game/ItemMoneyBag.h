#pragma once
#include "GameObject.h"
#include "Brick.h"


#define ITEM_MONEY_BAG_BBOX_WIDTH		12
#define ITEM_MONEY_BAG_BBOX_HEIGHT	14

class ItemMoneyBag:public CGameObject
{
public:
	ItemMoneyBag();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

