#pragma once
#include "GameObject.h"
#include "Portal.h"
#include "Utils.h"
#include "Game.h"
#include "Brick.h"
#include "Animations.h"
#include "Whip.h"
#include "Dagger.h"
#include "StairBottom.h"
#include "StairTop.h"
#include "VariousStair.h"
#include "BreakWall.h"
#include "Bat.h"
#include "Boomerang.h"
#include "SubWeapon.h"

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
#define SIMON_STATE_IDLE_UPSTAIR					11
#define SIMON_STATE_IDLE_DOWNSTAIR			12
#define SIMON_STATE_THROW							13

#define SIMON_STATE_DIE						400

#define SIMON_ANI_IDLE			0
#define SIMON_ANI_WALKING	1
#define SIMON_ANI_JUMP			2
#define SIMON_ANI_SIT				3
#define SIMON_ANI_ATTACK				4
#define SIMON_ANI_SIT_ATTACK		5
#define SIMON_ANI_GO_UPSTAIR						6
#define SIMON_ANI_GO_DOWNSTAIR				7
#define SIMON_ANI_ATTACK_UPSTAIR				8
#define SIMON_ANI_ATTACK_DOWNSTAIR		9

#define SIMON_ANI_DEFLECT							10

#define SIMON_ANI_IDLE_UPSTAIR		11
#define SIMON_ANI_IDLE_DOWNSTAIR	12
#define SIMON_ANI_THROW					13
#define SIMON_ANI_POWER_UP			14
#define SIMON_ANI_DIE							15

#define SIMON_GRAVITY						0.0006f	
#define SIMON_WALKING_SPEED			0.06f
#define SIMON_JUMP_SPEED_Y				0.2f
#define SIMON_GO_UPSTAIR_SPEED		0.03f
#define SIMON_DIE_DEFLECT_SPEED		0.5
#define SIMON_ATTACK_TIME				300
#define SIMON_AUTO_UPSTAIR_TIME			280
#define SIMON_AUTO_DOWNSTAIR_TIME		280
#define SIMON_DISCOLOR_TIME			700
#define SIMON_DYING_TIME			3000

#define SIMON_DEFLECT_TIME				800
#define SIMON_DEFLECT_SPEED_X			0.06f
#define SIMON_DEFLECT_SPEED_Y			0.18f
#define SIMON_UNTOUCHABLE_TIME	3000
#define SIMON_MAX_MANA				16

#define SIMON_BBOX_WIDTH			16
#define SIMON_BBOX_HEIGHT			30



/*
	Data struct for saving auto move event
*/
struct AutoInfo
{
	float vx;
	float vy;
	float xDes;				// For auto walking till reach a point
	float yDes;
	DWORD timeMove;		// For auto walking within a given time
};

class CSimon : public CGameObject
{
private:
	// HUD's infor
	int heart_quantity = 0;
	int score;

	static CSimon* __instance; 
	float start_x, start_y; // Initial position of simon at scene instead of (0,0)

	int  revieScene; // Handle player's revie scene

	float firstY; // Check if simon is falling down in the large distance
	int untouchable;
	DWORD untouchable_start;

	AutoInfo autoInfo;

	bool subWeapon = false;
	CSubWeapon* weapons;
	int currentSubWeapon;

	bool isStanding = false;
	bool powerUp = false;
	bool onMovingPlatform = false;
	bool atuo_crouching = false;
	bool autoWalk = false;
	bool using_stop_watch = false;

	DWORD discolorationTime = 0;
	DWORD auto_crouching_start = 0;
	DWORD auto_walk_start = 0;

	vector<LPGAMEOBJECT> ovObjects;		// overlapping objects	
public:

	/* Assigned to -1 if going downstairs
	 Assigned to 1 if going upstairs
	 Assigned to 0 if not on stairs*/
	int onStairs;
	CWhip* whip;	
	
	int GetSubWeapon() { return this->currentSubWeapon; }
	void SetSubWeapon(int weapon) { this->currentSubWeapon = weapon; }

	bool StopWatch() { return this->using_stop_watch; }
	void SetStopWatch(bool watch) { this->using_stop_watch = watch; }

	void SetBackScene(int scene_id) { this->revieScene = scene_id; }
	int GetBackScene() { return this->revieScene; }

	int GetHeartQuantity() { return this->heart_quantity; }
	int GetScore() { return this->score; }
	int GetHP() { return this->healthPoint; }

	bool PowingUp() { return this->powerUp; }
	
	CSimon(float x=0.0f, float y =0.0f);
	
	virtual void Update(DWORD dt, vector <LPGAMEOBJECT>* coObject = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	int GetUntouchable() { return this->untouchable; }

	void Render();
	void SetState(int state);	
	void Reset();
	bool isOnGround() { return vy == 0; }	
	bool lastFrameAttack();

	static CSimon* GetInstance();

	void GoUpStair();
	void GoDownStair();
	void ProceedOnStairs();

	void StartAutoWalk(float vx, float end_x);
	void StartAutoWalk(float vx, float vy, DWORD time);
	void ProceedAutoWalk();

	void ProceedOverlapping();
	int GetOverlapObjectSize() { return this->ovObjects.size(); }
};