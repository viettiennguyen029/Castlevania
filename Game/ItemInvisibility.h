#pragma once
#include "GameObject.h"

#define ITEM_INVISIBILITY_BBOX_WIDTH	13
#define ITEM_INVISIBILITY_BBOX_HEIGHT	16

class ItemInvisibility : public CGameObject
{
	DWORD start_visible = 0;
public:
	ItemInvisibility();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};