#pragma once
#include "GameObject.h"
#include "Simon.h"

#define CROWN_BBOX_WIDTH		16
#define CROWN_BBOX_HEIGHT		16
#define CROWN_MAX_Y					200
#define CROWN_ACTIVE_POINT_X 214
#define CROWN_GROWING_UP_SPEED	0.03f

class ItemCrown : public CGameObject
{
	DWORD start_visible = 0;
	public:
	ItemCrown();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving = false);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};