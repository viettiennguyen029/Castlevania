#pragma once

#include "GameObject.h"
#define CANDLE_STATE_BIG 0

#define CANDLE_BBOX_WIDTH 16
#define CANDLE_BBOX_HEIGHT 32
class CCandle : public CGameObject
{
public:
	CCandle();	
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};