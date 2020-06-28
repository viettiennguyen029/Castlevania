#pragma once
#include "GameObject.h"

class CHolyWater : public CGameObject
{
	static CHolyWater* __instance;
	vector<vector<float>> hitEffects; //a vector for saving the hit effects coordinates when the whip hits the target
	int startShow = 0;
	CAnimation* hitEffect = CAnimations::GetInstance()->Get(HIT_EFFECT_ANIMATION);
public:
	CHolyWater();
	static CHolyWater* GetInstance();
	void ShowHitEffect();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};