#pragma once
#include "GameObject.h"
#include "Simon.h"

#define BOSSBAT_BBOX_WIDTH		48
#define BOSSBAT_BBOX_HEIGHT	22

#define BOSSBAT_REST_TIME		1500
#define BOSSBAT_ATTACK_TIME		1000

#define BOSSBAT_GO_TO_REST_SPEED_VX		0.06f
#define BOSSBAT_GO_TO_REST_SPEED_VY		0.06f

#define BOSSBAT_MAX_SPEED_VX			0.2f
#define BOSSBAT_MAX_SPEED_VY			0.3f
#define BOSSBAT_FLY_UP_SPEED_VY			-0.004f

#define BOSSBAT_POINT_AWARD				1000
#define BOSSBAT_DEFAULT_HEALTH			16
#define BOSSBAT_DAMAGE					2

// Minimum space between Simon and the boss while it is resting
#define BOSSBAT_MINIMUM_DISTANCE_WITH_SIMON		120

class CBossBat : public CGameObject
{
	bool stop = false;
	bool sleeping = true;
	bool goingToRest = false;

	float central_x;		// Position of the the boss
	float central_y;

	float target_x;			// Position of the player
	float target_y;

	float rest_x;			// Position of the boss is going to rest
	float rest_y;

	DWORD rest_start = 0;
	DWORD attack_start = 0;

public:

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) ;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL, bool stopMoving = false) ;
	//void BeHit(int damage) ;

	void Render();

	void CalculateCentralPosition();
	void CalculateRestPlace();

	void FindTarget();
	bool IsSleeping() { return this->sleeping; }
	bool CanWakeUp();
	bool ReachedRestPlace();
	void ProceedAttacking();

	// Main actions
	void StartAttack();
	void GoToRest();
	void Rest();

	CBossBat();
};

