#pragma once
#include "GameObject.h"
#include "Animations.h"
// Whip 
#define NORMAL_WHIP		0
#define SHORT_CHAIN		1
#define LONG_CHAIN			2

#define WHIP_BBOX_HEIGHT					 6
#define SHORT_CHAIN_BBOX_WIDTH	24
#define LONG_CHAIN_BBOX_WIDTH		38

#define OFFSET_TOP									10
#define OFFSET_LEFT_LONG_CHAIN			15
#define OFFSET_LEFT_SHORT_CHAIN			25
#define OFFSET_RIGHT_CHAIN					70

#define HIT_EFFECT_LIFE_SPAN	100

class CWhip : public CGameObject
{
	int level;
	static CWhip* __instance;

	vector<vector<float>> hitEffects; //a vector for saving the hit effects coordinates when the whip hits the target
	int startShow = 0;
	CAnimation* hitEffect = CAnimations::GetInstance()->Get(HIT_EFFECT_ANIMATION);
public:
	CWhip();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL, bool stopMoving = false) ;
	virtual void Render() {}
	virtual void Render(int currentFrame);
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetWhipPosition(D3DXVECTOR2 simonPos, bool isStanding);
	void PowerUp();
	void ShowHitEffect();
	void SetDamage(int state);
	static CWhip* GetInstance();
};

