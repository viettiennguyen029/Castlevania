#pragma once
#include "GameObject.h"

#define EFFECT_LIFESPAN 300

#define FLAME_BBOX_WIDTH        8
#define FLAME_BBOX_HEIGHT   8

class HitEffect : public CGameObject
{
    DWORD  startToShow;
public:
    HitEffect();
    void Show();
    void End();
    virtual void Update(DWORD dt, vector <LPGAMEOBJECT>* coObjects) {};
    virtual void Render();
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};