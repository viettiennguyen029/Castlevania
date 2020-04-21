#pragma once
#include "GameObject.h"

// Whip
#define NORMAL_WHIP		0
#define SHORT_CHAIN		1
#define LONG_CHAIN		2

#define WHIP_BBOX_HEIGHT 15
#define WHIP_BBOX_WIDTH 15
#define LONG_CHAIN_BBOX_WIDTH		85
class CWhip: public CGameObject
{
public:
	CWhip();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() {};
	virtual void Render(int currentFrame);
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	// void Render(int currentID = -1);
	void SetWhipPosition(D3DXVECTOR2 simonPos);
	void PowerUp();

};

