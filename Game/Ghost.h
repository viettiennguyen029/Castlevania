#pragma once
#include "GameObject.h"

#define GHOST_STATE_FLYING					0
#define GHOST_STATE_ATTACK_PLAYER		1

#define GHOST_BBOX_WIDTH	16
#define GHOST_BBOX_HEIGHT	16


#define GHOST_FLYING_SPEED_VX 0.03
#define GHOST_FLYING_SPEED_VY 0.015

class CGhost : public CGameObject
{
private:
	//static CGhost* __instance;
	bool attacking;
	bool flyingup;
	float start_y;

	bool stop = false;
public:
	CGhost();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL, bool stopMoving = false);
	virtual void Render();

	void DetectPlayer();
};

class CSpawnGhost
{
	unordered_map<int, vector<LPGAMEOBJECT>> ghosts;

	static CSpawnGhost* __instance;
public:

	void Add(int obj_type, LPGAMEOBJECT ghost);
	void SetVisible();
	void Clear();
	LPGAMEOBJECT Get(int ghost);
	CSpawnGhost() { ; }
	static CSpawnGhost* GetInstance();
};