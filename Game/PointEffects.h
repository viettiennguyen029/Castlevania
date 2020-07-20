#pragma once
#include "GameObject.h"

#define POINT_EFFECT_100	0
#define POINT_EFFECT_400	1
#define POINT_EFFECT_700	2
#define POINT_EFFECT_1000	3
#define POINT_EFFECT_2000	4

#define POINT_EFFECT_LIFE_SPAN 300

class PointEffect : public CGameObject
{
private:
	DWORD start_visible = 0;
public: 
	PointEffect();
	virtual void SetState(int state);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

class PointEffects
{
private:
	vector <PointEffect*> points;
	static PointEffects* __instance;
public:	
	static PointEffects* GetInstance();
	void Add(PointEffect* point);
	void ShowPoint(int state, float x, float y);
	void Clear();
	PointEffects() { };
};