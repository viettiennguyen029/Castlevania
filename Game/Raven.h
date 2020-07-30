#pragma once
#include "GameObject.h"
#define RAVEN_STATE_IDLE			0
#define RAVEN_STATE_FLYING	1

#define RAVEN_BBOX_WIDTH		16
#define RAVEN_BBOX_HEIGHT	16

#define DISTANCE_TO_ACTIVE_RAVEN	138

#define RAVEN_FLYING_DOWN_SPEED	0.05f
#define RAVEN_ATTACK_VELOCITY		0.002f

#define RAVEN_DELAY_TIME		1000
#define RAVEN_DETECT_TIME		2000
#define RAVEN_ATTACK_TIME		700
class CRaven : public CGameObject
{
	bool stop = false; // To check if player is using stop watch or not

	int ny; // Direction: y

	float dist;

	bool detectPlayer = false; // find target
	bool reDetectPlayer = false;
	bool buffSpeed = false;
	bool doyouwanttobuildasnowman = false;

	LPGAMEOBJECT player;

	DWORD raven_change_state = 0;
	DWORD raven_find_target = 0;
	DWORD raven_want_to_play_again = 0;
	
public:
	CRaven();
	virtual void Render();
	virtual void SetState(int state);
	float CalcDistance(float x1, float y1, float x2, float y2);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL, bool stopMoving = false);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};