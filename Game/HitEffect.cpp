#include "HitEffect.h"
// #include "Items.h"

HitEffect::HitEffect()
{
    SetVisible(false);
}

void HitEffect::Show()
{
    startToShow = GetTickCount();
    if (GetTickCount() - startToShow > EFFECT_LIFESPAN)
    {
        this->End();
    }
}

void HitEffect::End()
{
    // CItems::GetInstance()->CheckAndDrop(this);
    SetVisible(false);
}

void HitEffect::Render()
{
    animation_set->at(0)->Render(x, y, -1);
}

void HitEffect::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x;
    top = y;
    right = left + FLAME_BBOX_WIDTH;
    bottom = top + FLAME_BBOX_HEIGHT;
}