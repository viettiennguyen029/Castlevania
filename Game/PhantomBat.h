#pragma once
#include "GameObject.h"

#define PHANTOM_BAT_STATE_INACTIVE	0
#define PHANTOM_BAT_STATE_ACTIVED	1

#define PHANTOM_BAT_BBOX_WIDTH		48
#define PHANTOM_BAT_BBOX_HEIGHT		22

#define PHANTOM_BAT_RETREATS_TIME			1500
#define PHANTOM_BAT_SWOOP_DOWN_TIME	1000

#define PHANTOM_BAT_RETREATS_VX		0.06f
#define PHANTOM_BAT_RETREATS_VY		0.06f

#define PHANTOM_BAT_MAX_SPEED_VX	0.2f
#define PHANTOM_BAT_MAX_SPEED_VY	0.2f

#define PHANTOM_BAT_BACK_TO_RETREATS_SPEED_VY	-0.004f

#define PHANTOM_BAT_MIN_DISTANCE_WITH_SIMON	150

class CPhantomBat : public CGameObject
{
	float start_x, start_y;

	float boss_x;			// Position of  the boss
	float boss_y;

	float player_x;		// Position of the player
	float player_y;

	float retreats_x;	//Position for the boss to take a retreats
	float retreats_y;

	bool inActive = true;
	bool retreats = false;

	DWORD retreat_start = 0; 
	DWORD swoopDown_start = 0;

public:
	bool IsActive();
	bool IsInRetreats();

	void GetBossPosition();
	void DetectPlayer();
	void CalcRetreatPoint();

	CPhantomBat(float x = 0.0f, float y = 0.0f);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) ;
	virtual void  Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	void GoToRetreats();
	void TakeARetreats();
	void SwoopDown();
	void ProceedAttacking();
};

