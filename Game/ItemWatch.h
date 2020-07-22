#pragma once
#include "GameObject.h"

#define ITEM_WATCH_BBOX_WIDTH	16
#define ITEM_WATCH_BBOX_HEIGHT	16

class ItemWatch : public CGameObject
{
	DWORD timeAppear = -1;
public:
	ItemWatch();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving = false);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
