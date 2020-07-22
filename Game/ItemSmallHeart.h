#pragma once
#include "GameObject.h"

#define ITEM_SMALL_HEART_BBOX_WIDTH	8
#define ITEM_SMALL_HEART_BBOX_HEIGHT	8

class ItemSmallHeart: public CGameObject
{
	float   vx_variability; // The variability of small heart vx during delta time
	DWORD start_visible = 0;
public:
	ItemSmallHeart();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving = false);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

