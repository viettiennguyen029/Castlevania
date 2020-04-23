#pragma once
#include "GameObject.h"
#include "Animations.h"
// Whip
#define NORMAL_WHIP		0
#define SHORT_CHAIN		1
#define LONG_CHAIN		2


#define WHIP_BBOX_HEIGHT 9
#define WHIP_BBOX_WIDTH 25

class CWhip : public CGameObject
{
public:
	CWhip();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render() {};
	virtual void Render(int currentFrame);
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	bool isColliding(float obj_left, float obj_top, float obj_right, float obj_bottom);
	// void Render(int currentID = -1);
	void SetWhipPosition(D3DXVECTOR2 simonPos);
	void PowerUp();

};

