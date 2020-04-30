#pragma once
#include "GameObject.h"

#define ITEM_BIG_HEART_BBOX_WIDTH		12
#define ITEM_BIG_HEART_BBOX_HEIGHT	10

class ItemBigHeart:public CGameObject
{
public:
	ItemBigHeart();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};