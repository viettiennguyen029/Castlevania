#pragma once
#include "GameObject.h"
#include "Portal.h"
#include "Utils.h"
#include "Game.h"

#define SIMON_GRAVITY				0.0007f	

#define SIMON_STATE_IDLE						0
#define SIMON_STATE_WALKING_RIGHT	100
#define SIMON_STATE_WALKING_LEFT		200
#define SIMON_STATE_JUMP						300
#define SIMON_STATE_DIE						400
#define SIMON_STATE_SIT							500
#define SIMON_STATE_ATTACK					600
#define SIMON_STATE_SIT_ATTACK			700

#define SIMON_ANI_IDLE_RIGHT			0
#define SIMON_ANI_WALKING_RIGHT	1
#define SIMON_ANI_JUMP_RIGHT			2
#define SIMON_ANI_SIT_RIGHT				3

#define SIMON_ANI_IDLE_LEFT				4
#define SIMON_ANI_WALKING_LEFT		5
#define SIMON_ANI_JUMP_LEFT			6
#define SIMON_ANI_SIT_LEFT				7
#define SIMON_ANI_DIE							8

#define SIMON_ANI_ATTACK_RIGHT		9
#define SIMON_ANI_ATTACK_LEFT		10
#define SIMON_ANI_SIT_ATTACK_RIGHT 11
#define SIMON_ANI_SIT_ATTACK_LEFT	12

#define SIMON_WALKING_SPEED			0.08f
#define SIMON_JUMP_SPEED_Y			0.22f
#define SIMON_DIE_DEFLECT_SPEED	0.5f


#define SIMON_BBOX_WIDTH				15
#define SIMON_BBOX_HEIGHT			30
class CSimon : public CGameObject
{
	bool isOnGround;

	DWORD startAttackingTime;
	DWORD startJjumpingTime;
public: 
	CSimon();

	virtual void Update(DWORD dt, vector <LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	
	bool IsOnGround() { return isOnGround; }

	void StartAttacking();
	void StartJumping();

	DWORD GetStartAttackTime() { return startAttackingTime; }
	DWORD GetStartJumpTime() { return startJjumpingTime; }

};