#pragma once
#include "GameObject.h"

#define ITEM_TIME_DESTROYED		3000
#define ITEM_FALLING_SPEED		0.1f

#define BIG_HEART 0
#define CHAIN		1
#define DAGGER		2

#define BIG_HEART_BBOX_WIDTH		12
#define BIG_HEART_BBOX_HEIGHT		10

#define CHAIN_BBOX_WIDTH				16
#define CHAIN_BBOX_HEIGHT			16

#define DAGGER_BBOX_WIDTH			16
#define DAGGER_BBOX_HEIGHT			10

class CItems : public CGameObject
{
	DWORD timeAppear;
public: 
	CItems();
	void SetItem(int itemId);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* Objects = NULL, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

