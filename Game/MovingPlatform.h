#pragma once
#include "GameObject.h"
#define MOVING_PLATFORM_BBOX_WIDTH  32
#define MOVING_PLATFORM_BBOX_HEIGHT 8

#define MOVING_PLATFORM_SPEED       0.03f
class CMovingPlatform: public CGameObject
{
public:
    CMovingPlatform();
    virtual void Render();
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

};