#pragma once
#include "GameObject.h"

#define ITEM_DAGGER_BBOX_WIDTH		16
#define ITEM_DAGGER_BBOX_HEIGHT	10

class ItemDagger :public CGameObject
{
public:
	ItemDagger();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

