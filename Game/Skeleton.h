#pragma once
#include "GameObject.h"
#include "Bone.h"

#define SKELETON_BBOX_WIDTH	16
#define SKELETON_BBOX_HEIGHT	32

#define SKELETON_STATE_IDLE		0
#define SKELETON_STATE_ACTIVE	1

class CSkeleton : public CGameObject
{
private:
	//CBones* bone;

	bool stop = false;
	bool canThrowBones = false;
	bool onGround = false;
	bool jumping = false;

	vector<LPGAMEOBJECT> bones; // a vector bones of skeleton
public:
	CSkeleton();

	virtual void Render();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL, bool stopMoving = false);

	vector <LPGAMEOBJECT> GetBones();
};