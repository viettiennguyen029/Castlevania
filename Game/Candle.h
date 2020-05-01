#pragma once

#include "PlayScence.h"
#include "GameObject.h"
#define CANDLE_STATE_BIG	 0
#define CANDLE_DESTROYED 1

#define CANDLE_BBOX_WIDTH	16
#define CANDLE_BBOX_HEIGHT 32

#define CANDLE_DESTROYED_EFFECT_TIME 300
class CCandle : public CGameObject
{
	// bool destroyed = false;
public:
	CCandle();	
	virtual void Render();
	// virtual void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};