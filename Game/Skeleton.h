#pragma once
#include "GameObject.h"
#include "Bone.h"

#define SKELETON_BBOX_WIDTH	16
#define SKELETON_BBOX_HEIGHT	32

#define SKELETON_STATE_IDLE		0
#define SKELETON_STATE_ACTIVE	1

class CSkeleton : public CGameObject
{
	//CBones* bone;
	static CSkeleton* __instance;

	bool stop = false;
	bool canThrowBones = false;

	vector<LPGAMEOBJECT> bones; // a vector bones of skeleton
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL, bool stopMoving = false);
	virtual void Render();

	CSkeleton();
	virtual void SetState(int state);

	static CSkeleton* GetInstance();
};