#pragma once
#include "GameObject.h"

#define ITEM_SMALL_HEART_BBOX_WIDTH	8
#define ITEM_SMALL_HEART_BBOX_HEIGHT	8

class ItemSmallHeart: public CGameObject
{
	float   vx_variability; // The variability of small heart vx during delta time
public:
	ItemSmallHeart();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

