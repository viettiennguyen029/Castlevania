#pragma once
#include "GameObject.h"

#define BLACK_KNIGHT_STATE_WALKING	0
#define BLACK_KNIGHT_STATE_DIE			1

#define BLACK_KNIGHT_BBOX_WIDTH	16
#define BLACK_KNIGHT_BBOX_HEIGHT	32

#define BLACK_KNIGHT_WALKING_SPEED	0.04f

class CBlack_Knight : public CGameObject	
{
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	CBlack_Knight();
	virtual void SetState(int state);
};

