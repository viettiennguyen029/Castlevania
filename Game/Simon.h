#pragma once
#include "GameObject.h"
#include "Portal.h"
#include "Utils.h"
#include "Game.h"
#include "Brick.h"
#include "Animations.h"
#include "Whip.h"
#include "Dagger.h"

#define SIMON_STATE_IDLE								0
#define SIMON_STATE_WALKING						1
#define SIMON_STATE_JUMP								2
#define SIMON_STATE_SIT									3
#define SIMON_STATE_ATTACK							4
#define SIMON_STATE_SIT_ATTACK					5
#define SIMON_STATE_GO_UPSTAIR					6
#define SIMON_STATE_GO_DOWNSTAIR			7
#define SIMON_STATE_ATTACK_UPSTAIR			8
#define SIMON_STATE_ATTACK_DOWNSTAIR		9
#define SIMON_STATE_DEFLECT							10

#define SIMON_STATE_DIE						400

#define SIMON_ANI_IDLE			0
#define SIMON_ANI_WALKING	1
#define SIMON_ANI_JUMP			2
#define SIMON_ANI_SIT				3

#define SIMON_ANI_ATTACK				4
#define SIMON_ANI_SIT_ATTACK		5

#define SIMON_ANI_GO_UPSTAIR						6
#define SIMON_ANI_GO_DOWNSTAIR				7
#define SIMON_STATE_ATTACK_UPSTAIR			8
#define SIMON_STATE_ATTACK_DOWNSTAIR		9

#define SIMON_STATE_DEFLECT							10

#define SIMON_GRAVITY						0.0005f	
#define SIMON_WALKING_SPEED			0.06f
#define SIMON_JUMP_SPEED_Y				0.18f
#define SIMON_GO_UPSTAIR_SPEED		0.04f
#define SIMON_DIE_DEFLECT_SPEED		0.5
#define SIMON_ATTACK_TIME				300

#define SIMON_BBOX_WIDTH			15
#define SIMON_BBOX_HEIGHT			30

class CSimon : public CGameObject
{
	static CSimon* __instance; // Singleton Patern
	float start_x, start_y; // Initial position of simon at scene instead of (0,0)
public:

	CWhip* whip;	
	CDagger* dagger;

	bool isStanding;
	bool subWeapon = false;

	CSimon(float x=0.0f, float y =0.0f);

	virtual void Update(DWORD dt, vector <LPGAMEOBJECT>* coObject = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void Render();
	void SetState(int state);	
	void Reset();
	bool isOnGround() { return vy == 0; }	

	static CSimon* GetInstance();
};