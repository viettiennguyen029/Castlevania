#pragma once
#include "GameObject.h"

#define BOOMERANG_BBOX_WIDTH		16
#define BOOMERANG_BBOX_HEIGHT	12

class CBoomerang: public CGameObject
{
	DWORD turnoverDelayTime=0;
	bool turnOver = false;

	vector<vector<float>> hitEffects; //a vector for saving the hit effects coordinates when the whip hits the target
	int startShow = 0;
	CAnimation* hitEffect = CAnimations::GetInstance()->Get(HIT_EFFECT_ANIMATION);

public:
	CBoomerang();
	void SetTurnOver(bool turnOver) { this->turnOver = turnOver; }
	void ShowHitEffect();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving = false);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

