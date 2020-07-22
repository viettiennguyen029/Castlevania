#pragma once
#include "GameObject.h"

#define HOLY_WATER_STATE_NORMAL	0
#define HOLY_WATER_STATE_BURN	1

#define HOLY_WATER_NORMAL_BBOX_WIDTH	 8
#define HOLY_WATER_NORMAL_BBOX_HEIGHT	 8

#define HOLY_WATER_BURN_BBOX_WIDTH	16
#define HOLY_WATER_BURN_BBOX_HEIGHT	13

#define  HOLY_WATER_VY		0.0002
#define HOLY_WATER_SPEED_VX	0.1

#define BURNING_TIME		2400
class CHolyWater : public CGameObject
{
	bool burning = false;
	DWORD burning_start = 0;

	static CHolyWater* __instance;
	vector<vector<float>> hitEffects; //a vector for saving the hit effects coordinates when the whip hits the target
	int startShow = 0;
	CAnimation* hitEffect = CAnimations::GetInstance()->Get(HIT_EFFECT_ANIMATION);

public:
	CHolyWater();
	static CHolyWater* GetInstance();
	void ShowHitEffect();
	virtual void Render();
	virtual void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving = false);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};