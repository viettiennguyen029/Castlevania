#pragma once

#include "GameObject.h"

class CCandle : public CGameObject
{
public:
	CCandle();	
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};