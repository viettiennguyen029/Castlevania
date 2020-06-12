#pragma once
#include "GameObject.h"

#define BLACK_KNIGHT_STATE_WALKING	0
#define BLACK_KNIGHT_STATE_DIE			1

#define BLACK_KNIGHT_BBOX_WIDTH	16
#define BLACK_KNIGHT_BBOX_HEIGHT	32

#define BLACK_KNIGHT_WALKING_SPEED	0.018f

class CBlack_Knight : public CGameObject	
{
	float start_x;
	float start_y;
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects= NULL);
	virtual void Render();

	CBlack_Knight(float x=0.0f, float y=0.0f);
	virtual void SetState(int state);
};

