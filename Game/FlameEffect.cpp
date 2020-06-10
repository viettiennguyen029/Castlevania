#include "FlameEffect.h"

FlameEffect* FlameEffect::__instance = NULL;
void FlameEffect::Add(HitEffect* flame)
{
    flame->SetVisible(false);
    flames.push_back(flame);
}

void FlameEffect::Show(LPGAMEOBJECT obj)
{
    // Detecting the central point of the Object's Bounding box
    float l, t, r, b;
    obj->GetBoundingBox(l, t, r, b);
    float xF = (l + r) / 2;
    float yF = (t + b) / 2;

    HitEffect* hitEffect;

    for (UINT i = 0; i < flames.size(); ++i)
    {
        hitEffect = flames[i];
        if (hitEffect->isVisible() == false)
       {
            hitEffect->SetVisible(true);
            hitEffect->Show();

            // The item ID was held in the flame effect
            hitEffect->SetItemId(obj->GetItemId());

            // The Effect is rendered at the  central point of the Object
            hitEffect->SetPosition(xF - FLAME_BBOX_WIDTH / 2, yF - FLAME_BBOX_HEIGHT / 2);
            break;
        }
    }
}
FlameEffect* FlameEffect::GetInstance()
{
    if (__instance == NULL)
        __instance = new FlameEffect();

    return __instance;
}