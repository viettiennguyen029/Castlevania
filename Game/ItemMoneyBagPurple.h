#pragma once
#pragma once
#include "GameObject.h"
#include "Brick.h"

#define ITEM_MONEY_BAG_BBOX_WIDTH		12
#define ITEM_MONEY_BAG_BBOX_HEIGHT	14
class ItemMoneyBagPurple : public CGameObject
{
public: 
	ItemMoneyBagPurple();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving = false);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};